//
// Created by erik on 9/19/16.
//

#include <cmath>
#include <iostream>
#include "Combat.h"
#include "Spawn.h"
#include "core/GameState.h"
#include "game/Starship.h"
#include "game/Projectile.h"
#include "game/systems.h"

namespace spatacs
{
    namespace events
    {
        FireWeapon::FireWeapon(std::uint64_t shooter, std::uint64_t target, std::uint64_t weapon) :
                ShipEvent( shooter ), mTarget( target ), mWeaponId(weapon)
        {
        }

        void FireWeapon::applyToShip(Starship& shooter, EventContext& context) const
        {
            // check that the target still exists
            if(!context.state.hasObject( mTarget ))
                return;

            auto& target = context.state.getShip( mTarget );
            auto& wpn = shooter.getWeapon( mWeaponId );
            auto tpos = target.position();
            auto tvel = target.velocity();
            auto ev = fireWeapon(shooter, tpos, tvel, wpn);
            if(ev)
                context.events.push_back( std::move(ev) );
        }

        auto FireWeapon::fireWeapon(Starship& shooter, const length_vec& tpos,
                                    const velocity_vec& tvel, game::ComponentEntity& weapon) const -> EventPtr
        {
            if (weapon.get<game::Timer>().time > 0)
                return nullptr;

            // reset timer
            weapon.get<game::Timer>().time = 60 / weapon.get<game::ProjectileWpnData>().mRPM;

            // get the ammo storage
            game::AmmoStorage::Ammo* ammo = nullptr;
            shooter.components().apply(
                    core::make_system<game::ComponentEntity, game::AmmoStorage>(
                            [&](const game::ComponentEntity& ety, game::AmmoStorage& as) mutable
                    {
                        auto aptr = &as.getAmmo(weapon.get<game::ProjectileWpnData>().mAmmo);
                        if(aptr->amount > 0)
                            ammo = aptr;
                    })
            );
            if(ammo == nullptr)
                return nullptr;

            speed_t muzzle_speed = sqrt(2.0 * ammo->data.charge / ammo->data.mass);
            weapon.get<game::WeaponAimData>().muzzle_velocity = muzzle_speed;

            // aiming
            game::Aiming aim(tpos - shooter.position(), tvel - shooter.velocity());
            aim(weapon);

            ammo->amount -= 1;

            auto d = length(aim.aim_pos());
            // modulate the speed with +-5%
            double speed_factor = ((rand() % 101 - 50) / 1000.0 + 1.0);
            auto vel = aim.speed() * aim.aim_pos() / d * speed_factor;
            return EventPtr(new SpawnProjectile(shooter.id(), shooter.position(),
                                                vel + shooter.velocity(),
                                                ammo->data.mass,
                                                0.0_m, ammo->data.damage));
        }
        // -------------------------------------------------------------------------------------------------------------

        SetWeaponAmmo::SetWeaponAmmo(std::uint64_t ship, std::uint64_t wpn, std::string ammo):
            ShipEvent(ship),
            mAmmo(ammo),
            mWeaponId(wpn)
        {

        }

        void SetWeaponAmmo::applyToShip(Starship& ship, EventContext& context) const
        {
            ship.getWeapon(mWeaponId).get<game::ProjectileWpnData>().mAmmo = mAmmo;
        }

        // -------------------------------------------------------------------------------------------------------------

        Hit::Hit(const Starship& ship, const game::Projectile& proj) :
                ShipEvent( ship.id() )
        {
            auto relvel = length(ship.velocity() - proj.velocity());
            auto dmg = 0.5 * relvel * relvel * proj.mass();
            double dval = dmg / 20000.0_kJ;
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
            auto relvel = length(ship.velocity() - proj.velocity());
            auto dmg = 0.5 * relvel * relvel * proj.mass();
            double dval = dmg / 20000.0_kJ;
            mDamage = proj.damage();
            mDamage.kinetic += dval;
            mProjectileID = proj.id();
        }

        void applyDamage(double& damage, double factor, double& target);

        void HitShield::applyToShip(Starship& target, EventContext& context) const
        {
            // next, let shield stop the HE part. HE only does 50% damage against shield
            double so = mDamage.shield_overload;
            double he = mDamage.high_explosive;
            double kd = mDamage.kinetic;
            double os = target.shield();

           // first, apply shield overload. This affects the whole shield
            applyDamage(so, 1.0, os);

            // only a portion of the shield can block at any given time
            /// \todo make this radius dependent?
            double block = os * 0.2;
            os -= block;
            applyDamage(kd, 0.5, block);
            applyDamage(he, 0.5, block);
            target.setShield( os + block );
            auto& proj = context.state.getProjectile(mProjectileID);
            if(kd == 0)
            {
                proj.expire();
            } else
            {
                /// \todo currently, we cannot change anything in physics code, so
                /// it is impossible to make the projectile change course.
            }
        }


        // -------------------------------------------------------------------------------------------------------------

        Damage::Damage(std::uint64_t ship, const game::Damage& damage) :
                ShipEvent( ship ), mDamage( damage )
        {
        }

        void Damage::applyToShip(Starship& target, EventContext& context) const
        {
            double he = mDamage.high_explosive;
            double kd = mDamage.kinetic;
            assert(mDamage.armour_pierce <= 1.0);
            double ap = mDamage.kinetic * mDamage.armour_pierce;

            // ---------------------------------------------------------------------------------
            double oh = target.armour();
            auto area = 4 * 3.1415 * target.radius() * target.radius();
            auto armour_density = scalar_t(oh) / area;
            double rel_strength = scalar_t(ap + 0.2) / (armour_density * 65.0_m * 65.0_m);
            if(rel_strength > 1)
                rel_strength = 1.0;

            int pierce_prob = int(100 * rel_strength);
            std::cout << pierce_prob << "\n";

            if(rand() % 100 < pierce_prob)
            {
                // in the piercing case, only 50% of the AP
                // damage damage the armour, and the remaining KD
                // goes to structure
                ap *= 0.5;
                double old_ap = ap;
                applyDamage(ap, 1.0, oh);
                kd -= old_ap - ap;
            } else {
                double old_ap = ap;
                applyDamage(ap, 1.0, oh);
                kd -= old_ap - ap;
                applyDamage(kd, 0.33, oh);
            }
            applyDamage(he, 0.20, oh);
            target.setArmour(oh);

            // ---------------------------------------------------------------------------------

            // Ok, now all that remains goes towards the ship structure
            double damage = he + kd;
            target.dealDamage( damage );
        }

        void applyDamage(double& damage, double factor, double& target) {
            double dmg = std::min(target, damage*factor);
            damage -= dmg / factor;
            target -= dmg;
        }
    }
}