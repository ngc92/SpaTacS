//
// Created by erik on 9/19/16.
//

#ifndef SPATACS_COMBAT_H_INCLUDED
#define SPATACS_COMBAT_H_INCLUDED

#include "ShipEvent.h"
#include "vec.h"
#include "game/Damage.h"
#include "game/components.h"

namespace spatacs {

    namespace game
    {
        class Projectile;
    }
    namespace events {
        class FireWeapon : public ShipEvent
        {
        public:
            FireWeapon( std::uint64_t shooter, std::uint64_t target, std::uint64_t wpn );

            std::uint64_t target() const { return mTarget; }
        private:

            virtual void applyToShip(Starship& shooter, EventContext& context) const override;
            EventPtr fireWeapon(Starship& shooter, const length_vec& tpos, const velocity_vec& tvel, game::ComponentEntity& weapon) const;

            std::uint64_t mTarget;
            std::uint64_t mWeaponId;
        };

        class SetWeaponAmmo : public ShipEvent
        {
        public:
            SetWeaponAmmo(std::uint64_t ship, std::uint64_t wpn, std::string ammo);

            const std::string& ammo() const { return mAmmo; }
        private:

            void applyToShip(Starship& shooter, EventContext& context) const override;

            std::string   mAmmo;
            std::uint64_t mWeaponId;
        };

        class SetSystemActivity : public ShipEvent
        {
        public:
            SetSystemActivity(std::uint64_t ship, std::uint64_t system, double activity);
        private:

            void applyToShip(Starship& shooter, EventContext& context) const override;
            std::uint64_t mSystem;
            double mActivity;
        };

        class HitShield : public ShipEvent
        {
        public:
            HitShield(const Starship& ship, const game::Projectile& proj);
        private:
            virtual void applyToShip(Starship& target, EventContext& context) const override;

            game::Damage mDamage;
            std::uint64_t mProjectileID;
            velocity_vec mShipVel;
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
            Damage(std::uint64_t ship, const game::Damage& damage);

            game::Damage damage() const { return mDamage; }
        private:
            virtual void applyToShip(Starship& target, EventContext& context) const override;

            game::Damage mDamage;
        };
    }
}
#endif //SPATACS_COMBAT_H_INCLUDED
