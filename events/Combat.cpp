//
// Created by erik on 9/19/16.
//

#include <cmath>
#include <iostream>
#include "Combat.h"
#include "Spawn.h"
#include "game/GameState.h"
#include "game/Starship.h"
#include "game/Projectile.h"
#include "physics/PhysicsWorld.h"
#include "core/ecs/EntityManager.h"
#include "core/ecs/EntityHandle.h"
#include "game/systems/Weapon.h"
#include "game/systems/Ammunition.h"

namespace spatacs
{
    namespace events
    {
        FireWeapon::FireWeapon(game::ObjectID shooter, game::ObjectID target, game::CompEntID weapon) :
                ShipEvent( shooter ), mTarget( target ), mWeaponId(weapon)
        {
        }

        void FireWeapon::applyToShip(Starship& shooter, EventContext& context) const
        {
            // check that the target still exists
            if(!context.state.hasObject( mTarget ))
                return;

            auto& target = context.state.getObject( mTarget );
            auto wpn     = shooter.components().get(mWeaponId);
            auto tpos    = target.position();
            auto tvel    = target.velocity();
            auto ev      = fireWeapon(shooter, tpos, tvel, wpn);
            if(ev)
                context.events.push_back( std::move(ev) );
        }

        auto FireWeapon::fireWeapon(Starship& shooter, const length_vec& tpos,
                                    const velocity_vec& tvel, game::SubsystemHandle& weapon) const -> EventPtr
        {
            if (weapon.get<game::Timer>().time > 0.0_s)
                return nullptr;

            // reset timer
            weapon.get<game::Timer>().time = 60.0_s / (double)weapon.get<game::ProjectileWpnData>().mRPM;

            // get the ammo storage
            game::systems::GetAmmunition getter{weapon.get<game::ProjectileWpnData>().mAmmo};
            shooter.components().apply(getter);
            if(!getter.has())
                return nullptr;

            speed_t muzzle_speed = sqrt(2.0 * getter.ammo_data().charge / getter.ammo_data().mass);
            weapon.get<game::WeaponAimData>().muzzle_velocity = muzzle_speed;

            // aiming
            game::systems::Aiming aim(tpos - shooter.position(), tvel - shooter.velocity());
            shooter.components().apply_to(weapon.id(), aim);

            auto d = length(aim.aim_pos());
            // modulate the speed with +-2.5%
            double speed_factor = ((rand() % 101 - 50) / 2000.0 + 1.0);
            auto vel = aim.speed() * aim.aim_pos() / d * speed_factor;
            return EventPtr(new SpawnProjectile(shooter.id(), shooter.position(),
                                                vel + shooter.velocity(),
                                                getter.ammo_data().mass,
                                                0.0_m, getter.ammo_data().damage));
        }
        // -------------------------------------------------------------------------------------------------------------

        SetWeaponAmmo::SetWeaponAmmo(game::ObjectID ship, game::CompEntID wpn, std::string ammo):
            ShipEvent(ship),
            mAmmo(ammo),
            mWeaponId(wpn)
        {

        }

        void SetWeaponAmmo::applyToShip(Starship& ship, EventContext& context) const
        {
            ship.components().get(mWeaponId).get<game::ProjectileWpnData>().mAmmo = mAmmo;
        }

        SetSystemActivity::SetSystemActivity(game::ObjectID ship, game::CompEntID system, double activity)
                : ShipEvent(ship), mSystem(system), mActivity(activity)
        {}

        void SetSystemActivity::applyToShip(ShipEvent::Starship& ship, EventContext& context) const
        {
            if(ship.components().is_alive(mSystem)) {
                auto sys = ship.components().get(mSystem);
                if(sys.has<game::Activity>())
                {
                    sys.get<game::Activity>().set(mActivity);
                }
            }
        }


        // -------------------------------------------------------------------------------------------------------------

        Hit::Hit(const Starship& ship, const game::Projectile& proj) :
                ShipEvent( ship.id() )
        {
            auto relvel = length(ship.velocity() - proj.velocity());
            auto dmg = 0.5 * relvel * relvel * proj.mass();
            double dval = dmg / 20000.0_kJ; /// \todo magic constant here
            mDamage = proj.damage();
            mDamage.kinetic += dval;
        }

        void Hit::applyToShip(Starship& target, EventContext& context) const
        {
            context.events.push_back( std::make_unique<Damage>( target.id(), mDamage ) );
        }

        // -------------------------------------------------------------------------------------------------------------

        HitShield::HitShield(const Starship& ship, const game::Projectile& proj) :
                ShipEvent( ship.id() )
        {
            mShipVel = ship.velocity();
            auto relvel = length(ship.velocity() - proj.velocity());
            auto dmg = 0.5 * relvel * relvel * proj.mass();
            double dval = dmg / 20000.0_kJ;
            mDamage = proj.damage();
            mDamage.kinetic += dval;
            mProjectileID = proj.id();
        }

        using game::applyDamage;

        void HitShield::applyToShip(Starship& target, EventContext& context) const
        {
            auto effect = game::getShieldDamage(mDamage, target.shield());
            // mDamage = effect.remaining;
            target.setShield( target.shield() - effect.applied );
            auto& proj = context.state.getObject(mProjectileID);
            if(effect.remaining.kinetic == 0)
            {
                dynamic_cast<game::Projectile&>(proj).expire();
            } else
            {
                auto pid = proj.physics_id();
                auto ksp = sqrt(2 * effect.remaining.kinetic * 20000.0_kJ / proj.mass());
                auto relvel = proj.velocity() - mShipVel;
                double factor = ksp / length(relvel);
                if(factor != 1) {
                    relvel *= factor;
                    relvel += mShipVel;
                    context.world.applyImpulse(pid, (relvel - proj.velocity()) * proj.mass());
                }

                /// \todo the shield should apply force (at least in part) radially, i think.
            }
            context.notifications.push_back( game::events::ShieldAbsorbtion(target.id(), effect.applied) );
        }


        // -------------------------------------------------------------------------------------------------------------

        Damage::Damage(game::ObjectID ship, const game::Damage& damage) :
                ShipEvent( ship ), mDamage( damage )
        {
        }

        void Damage::applyToShip(Starship& target, EventContext& context) const
        {
            // damage armour
            auto armour_dmg = game::getArmourDamage(mDamage, target.armour(), target.radius());
            target.setArmour( target.armour() - armour_dmg.applied );

            // Ok, now all that remains goes towards the ship structure
            double damage = armour_dmg.remaining.high_explosive + armour_dmg.remaining.kinetic;
            target.dealDamage( damage );

            context.notifications.push_back( game::events::ReceiveDamage(target.id(), armour_dmg.applied + damage) );
        }
    }
}