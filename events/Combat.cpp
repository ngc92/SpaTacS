//
// Created by erik on 9/19/16.
//

#include <cmath>
#include <iostream>
#include "Combat.h"
#include "Spawn.h"
#include "core/GameState.h"
#include "core/components/ShieldGenerator.h"
#include "core/components/Hull.h"
#include "core/components/IWeapon.h"

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
            // dead ships don't shoot
            if(!shooter.alive())
                return;

            auto& target = context.state.getShip( mTarget );
            auto& wpn = shooter.getWeapon( mWeaponId );
            auto ev = fireWeapon( shooter, target, wpn );
            if(ev)
                context.events.push_back( std::move(ev) );

         }

        auto FireWeapon::fireWeapon(Starship& shooter, const Starship& target,  core::IWeapon& weapon) const -> EventPtr
        {
            // check that weapon is ready
            if(!weapon.ready())
                return nullptr;

            // let the weapon create a projectile
            auto oproj = weapon.fire( target.position() - shooter.position(), target.velocity() - shooter.velocity() );

            if( oproj) {
                auto& shot = oproj.get();
                // update position and velocity according to shooter state

                return EventPtr(new SpawnProjectile(shooter.id(), shooter.position(), shot.vel + shooter.velocity(), 0.2_kg,
                                                    0.0_m, shot.damage));
            }
            // if no hit, return nullptr
            return nullptr;
        }

        // -------------------------------------------------------------------------------------------------------------

        SetWeaponMode::SetWeaponMode( std::uint64_t ship, std::uint64_t wpn, std::uint64_t mode ):
            ShipEvent(ship),
            mMode(mode),
            mWeaponId(wpn)
        {

        }

        void SetWeaponMode::applyToShip(Starship& ship, EventContext& context) const
        {
            ship.getWeapon(mWeaponId).setMode( mMode );
        }

        // -------------------------------------------------------------------------------------------------------------

        Hit::Hit(const Starship& ship, const core::Projectile& proj) :
                ShipEvent( ship.id() )
        {
            auto relvel = length(ship.velocity() - proj.velocity());
            auto dmg = relvel * relvel * proj.mass();
            mDamage = proj.damage();
            mDamage.kinetic += dmg.value;
        }

        void Hit::applyToShip(Starship& target, EventContext& context) const
        {
            context.events.push_back( std::make_unique<Damage>( target.id(), mDamage ) );
        }

        // -------------------------------------------------------------------------------------------------------------

        Damage::Damage(std::uint64_t ship, const core::Damage& damage) :
                ShipEvent( ship ), mDamage( damage )
        {
        }

        void Damage::applyToShip(Starship& target, EventContext& context) const
        {
            // next, let shield stop the HE part. HE only does 50% damage against shield
            float so = mDamage.shield_overload;
            float he = mDamage.high_explosive;
            float ap = mDamage.armour_piercing;
            float kd = mDamage.kinetic;
            float os = target.shield().shield();

            // first, apply shield overload
            applyDamage(so, 1.0, os);
            applyDamage(kd, 0.5, os);
            applyDamage(he, 0.5, os);
            applyDamage(ap, 0.33, os);
            target.getShield().setShield( os );

            // ---------------------------------------------------------------------------------

            float oh = target.hull().armour();

            applyDamage( he, 0.33, oh );
            applyDamage( kd, 0.33, oh );
            applyDamage( ap, 1.0, oh );
            target.getHull().setArmour( oh );

            // ---------------------------------------------------------------------------------

            // Ok, now all that remains goes towards the ship structure
            float damage = he + 0.33f * ap + 0.5f*kd;
            target.dealDamage( damage );
        }

        void Damage::applyDamage(float& damage, float factor, float& target) const {
            float dmg = std::min(target, damage*factor);
            damage -= dmg / factor;
            target -= dmg;
        }
    }
}