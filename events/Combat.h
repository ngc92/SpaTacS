//
// Created by erik on 9/19/16.
//

#ifndef SOI_COMBAT_H_INCLUDED
#define SOI_COMBAT_H_INCLUDED

#include "ShipEvent.h"
#include "vec.h"
#include "game/Damage.h"
#include "game/components/comps.h"

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

        class SetWeaponMode : public ShipEvent
        {
        public:
            SetWeaponMode( std::uint64_t ship, std::uint64_t wpn, std::uint64_t mode );

            std::uint64_t mode() const { return mMode; }
        private:

            void applyToShip(Starship& shooter, EventContext& context) const override;

            std::uint64_t mMode;
            std::uint64_t mWeaponId;
        };

        class HitShield : public ShipEvent
        {
        public:
            HitShield(const Starship& ship, const game::Projectile& proj);
        private:
            virtual void applyToShip(Starship& target, EventContext& context) const override;

            game::Damage mDamage;
            std::uint64_t mProjectileID;
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
#endif //SOI_COMBAT_H_INCLUDED
