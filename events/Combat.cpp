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
#include "core/System.h"

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

        class Aiming : public core::System<game::ComponentEntity, Aiming,
                core::Signature<game::WeaponAimData, game::Health>>
        {
        public:
            Aiming(length_vec position, velocity_vec velocity) :
                    mTargetPos(position),
                    mTargetVel(velocity)
            {
            }

            void apply(const game::ComponentEntity& ety, const game::WeaponAimData& aim,
                       const game::Health& health)
            {
                // perfect aim
                speed_t vel = aim.muzzle_velocity; // km/s
                auto d = length(mTargetPos);
                auto aim_pos = mTargetPos;

                for(unsigned i = 0; i < 3; ++i) {
                    aim_pos = mTargetPos + (d / vel) * mTargetVel;
                    auto old_d = d;
                    d = length(aim_pos);
                    //std::cout << "correct: " << i << " " << abs(d - old_d) << "\n";
                    if( abs(d - old_d) < 10.0_m )
                        break;
                }



                // miss-aim
                auto precision = aim.precision * (health.status() + 1) * .5;
                length_t aim_radius = d / precision;
                auto aim_radius_miss = (rand() % 1001 / 1000.0) * aim_radius;

                auto miss_base = meters(rand() % 1001 - 500.0, rand() % 1001 - 500.0, rand() % 1001 - 500.0);
                auto miss = perpendicular(miss_base, aim_pos);

                length_t n = length(miss);
                if( n == 0.0_m ) miss *= 0.0;
                else miss *= aim_radius_miss / n;

                mAimPos = aim_pos + miss;
                mMuzzleVel = vel;
            }

            const length_vec& aim_pos() const { return mAimPos; }
            const speed_t     speed() const { return mMuzzleVel; }
        private:
            length_vec mTargetPos;
            velocity_vec mTargetVel;

            length_vec mAimPos{.0, .0, .0};
            speed_t mMuzzleVel{.0};
        };

        auto FireWeapon::fireWeapon(Starship& shooter, const length_vec& tpos,
                                    const velocity_vec& tvel, game::ComponentEntity& weapon) const -> EventPtr
        {
            if (weapon.get<game::Timer>().time > 0)
                return nullptr;

            // reset timer
            weapon.get<game::Timer>().time = 60 / weapon.get<game::ProjectileWpnData>().mRPM;

            // get the ammo storage
            game::AmmoStorage::Ammo* ammo = nullptr;
            shooter.components().apply([&](game::ComponentEntity& ety) mutable
            {
                /// \todo search by name
                if(ety.has<game::AmmoStorage>())
                {
                    auto aptr = &ety.get<game::AmmoStorage>().getAmmo(weapon.get<game::ProjectileWpnData>().mAmmo);
                    if(aptr->amount > 0)
                        ammo = aptr;
                }
            });
            if(ammo == nullptr)
                return nullptr;

            speed_t muzzle_speed = sqrt(2.0 * ammo->data.charge / ammo->data.mass);
            weapon.get<game::WeaponAimData>().muzzle_velocity = muzzle_speed;

            // aiming
            Aiming aim(tpos - shooter.position(), tvel - shooter.velocity());
            aim(weapon);

            ammo->amount -= 1;

            auto d = length(aim.aim_pos());
            auto vel = aim.speed() * aim.aim_pos() / d;
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
            auto relvel = length(ship.velocity() - proj.velocity()) / 1000.0;
            auto dmg = relvel * relvel * proj.mass();
            mDamage = proj.damage();
            mDamage.kinetic += dmg.value;
        }

        void Hit::applyToShip(Starship& target, EventContext& context) const
        {
            context.events.push_back( std::make_unique<Damage>( target.id(), mDamage ) );
        }

        // -------------------------------------------------------------------------------------------------------------

        HitShield::HitShield(const Starship& ship, const game::Projectile& proj) :
                ShipEvent( ship.id() )
        {
            auto relvel = length(ship.velocity() - proj.velocity()) / 1000.0;
            auto dmg = relvel * relvel * proj.mass();
            mDamage = proj.damage();
            mDamage.kinetic += dmg.value;
            mProjectileID = proj.id();
        }

        void applyDamage(float& damage, float factor, float& target);

        void HitShield::applyToShip(Starship& target, EventContext& context) const
        {
            // next, let shield stop the HE part. HE only does 50% damage against shield
            float so = mDamage.shield_overload;
            float he = mDamage.high_explosive;
            float ap = mDamage.armour_piercing;
            float kd = mDamage.kinetic;
            float os = target.shield();

            // first, apply shield overload
            applyDamage(so, 1.0, os);
            applyDamage(kd, 0.5, os);
            applyDamage(he, 0.5, os);
            applyDamage(ap, 0.33, os);
            target.setShield( os );
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
            // next, let shield stop the HE part. HE only does 50% damage against shield
            float so = mDamage.shield_overload;
            float he = mDamage.high_explosive;
            float ap = mDamage.armour_piercing;
            float kd = mDamage.kinetic;
            float os = target.shield();

            // first, apply shield overload
            applyDamage(so, 1.0, os);
            applyDamage(kd, 0.5, os);
            applyDamage(he, 0.5, os);
            applyDamage(ap, 0.33, os);
            target.setShield( os );

            // ---------------------------------------------------------------------------------

            float oh = target.armour();

            applyDamage( he, 0.33, oh );
            applyDamage( kd, 0.33, oh );
            applyDamage( ap, 1.0, oh );
            target.setArmour( oh );

            // ---------------------------------------------------------------------------------

            // Ok, now all that remains goes towards the ship structure
            float damage = he + 0.33f * ap + 0.5f*kd;
            target.dealDamage( damage );
        }

        void applyDamage(float& damage, float factor, float& target) {
            float dmg = std::min(target, damage*factor);
            damage -= dmg / factor;
            target -= dmg;
        }
    }
}