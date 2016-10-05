//
// Created by erik on 9/19/16.
//

#ifndef SOI_WEAPON_H
#define SOI_WEAPON_H

#include <physics/units.h>
#include "IWeapon.h"
#include "core/Damage.h"

namespace spatacs
{
    namespace core
    {
        class ProjectileWeapon : public IWeapon
        {
        public:
            void onStep(Starship& ship);

            ProjectileWeapon( const ptree& props );
            ~ProjectileWeapon() = default;

            ProjectileWeapon* clone() const override;

            double precision() const;

            bool  ready() const override;
            float strength(length_t distance, float xsec) const override;
            float hit_chance(length_t distance, float xsec) const override;

            virtual void setMode(std::uint64_t mode) override;

            virtual uint64_t mode() const override;

            boost::optional<ShotData> fire(const length_vec& delta_p,
                                             const velocity_vec& delta_v) override;
        private:
            enum Mode
            {
                AP_MODE,
                HE_MODE,
                SO_MODE
            } mMode = HE_MODE;
            Damage mDamage;
            double mPrecision;
            float mRPM;
            speed_t mMuzzleVelocity;

            float mReload = 0;
        };
    }
}

#endif //SOI_WEAPON_H
