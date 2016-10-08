//
// Created by erik on 9/19/16.
//

#ifndef SOI_COMBAT_H_INCLUDED
#define SOI_COMBAT_H_INCLUDED

#include "ShipEvent.h"
#include "vec.h"
#include "core/Damage.h"

namespace spatacs {

    namespace core
    {
        class IWeapon;
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
            EventPtr fireWeapon(Starship& shooter, const length_vec& tpos, const velocity_vec& tvel, core::IWeapon& weapon) const;

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

        class Hit : public ShipEvent
        {
        public:
            Hit(const Starship& ship, const core::Projectile& proj);
        private:
            virtual void applyToShip(Starship& target, EventContext& context) const override;

            core::Damage mDamage;
        };

        class Damage : public ShipEvent
        {
        public:
            Damage(std::uint64_t ship, const core::Damage& damage);

            core::Damage damage() const { return mDamage; }
        private:
            virtual void applyToShip(Starship& target, EventContext& context) const override;

            void applyDamage(float& damage, float factor, float& target) const;

            core::Damage mDamage;
        };
    }
}
#endif //SOI_COMBAT_H_INCLUDED
