//
// Created by erik on 9/19/16.
//

#ifndef SPATACS_COMBAT_H_INCLUDED
#define SPATACS_COMBAT_H_INCLUDED

#include "ShipEvent.h"
#include "vec.h"
#include "game/Damage.h"
#include "game/systems/fwd.h"

namespace spatacs {

    namespace game
    {
        class Projectile;
        class Shield;
    }
    namespace events {
        class FireWeapon : public ShipEvent
        {
        public:
            FireWeapon(game::ObjectID shooter, game::ObjectID target, game::CompEntID wpn);

            game::ObjectID target() const { return mTarget; }
        private:

            virtual void applyToShip(Starship& shooter, EventContext& context) const override;
            EventPtr fireWeapon(Starship& shooter, const length_vec& tpos, const velocity_vec& tvel,
                                game::SubsystemHandle& weapon) const;

            game::ObjectID mTarget;
            game::CompEntID mWeaponId;
        };

        class SetWeaponAmmo : public ShipEvent
        {
        public:
            SetWeaponAmmo(game::ObjectID ship, game::CompEntID wpn, std::string ammo);

            const std::string& ammo() const { return mAmmo; }
        private:

            void applyToShip(Starship& shooter, EventContext& context) const override;

            std::string     mAmmo;
            game::CompEntID mWeaponId;
        };

        class SetSystemActivity : public ShipEvent
        {
        public:
            SetSystemActivity(game::ObjectID ship, game::CompEntID system, double activity);
        private:

            void applyToShip(Starship& shooter, EventContext& context) const override;
            game::CompEntID mSystem;
            double          mActivity;
        };

        class HitShield : public IEvent
        {
        public:
            HitShield(const game::Starship& ship, const game::Projectile& proj);
        private:
            void apply(EventContext& context) const override;

            game::Damage   mDamage;
            game::ObjectID mProjectileID;
            game::ObjectID mShieldID;
            velocity_vec   mShipVel;
        };

        class Hit : public ShipEvent
        {
        public:
            Hit(const Starship& ship, const game::Projectile& proj);
        private:
            virtual void applyToShip(Starship& target, EventContext& context) const override;

            game::Damage mDamage;
        };

        class Damage : public ShipEvent
        {
        public:
            Damage(game::ObjectID ship, const game::Damage& damage);

            game::Damage damage() const { return mDamage; }
        private:
            virtual void applyToShip(Starship& target, EventContext& context) const override;

            game::Damage mDamage;
        };
    }
}
#endif //SPATACS_COMBAT_H_INCLUDED
