//
// Created by erik on 9/20/16.
//

#ifndef SOI_RAYWEAPON_H
#define SOI_RAYWEAPON_H


#include <physics/units.h>
#include "IWeapon.h"

namespace spatacs
{
    namespace core
    {
        class RayWeapon : public IWeapon
        {
        public:
            void onStep();

            ~RayWeapon() override = default;

            RayWeapon* clone() const override;

            bool  ready() const override;

            double precision() const;

            float strength(length_t distance, float xsec) const override;
            float hit_chance(length_t distance, float xsec) const override;

            virtual void setMode(std::uint64_t mode) override;

            virtual uint64_t mode() const override;

            virtual boost::optional<ShotData> fire(const length_vec& delta_p, const velocity_vec& delta_v) override;

        private:
            float mStrength   = 1;
            double mPrecision  = 10;
            double mDispLength = 40; /// \todo this is actually the inverse
            float mBurstLength = 1;
            float mEnergyPerDamage = 1;


            bool mReady = true;
            float mCharge = 0;
            bool mRecharging = false;

            // cached miss data
            Vec mMissVector;
            float mMissFactor = 0;
        };
    }
}


#endif //SOI_RAYWEAPON_H
