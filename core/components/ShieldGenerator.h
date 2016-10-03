//
// Created by erik on 9/21/16.
//

#ifndef SOI_SHIELDGENERATOR_H
#define SOI_SHIELDGENERATOR_H

#include "vec.h"
#include "IComponent.h"

namespace spatacs {
    namespace core {
        class ShieldGenerator : public IComponent
        {
        public:
            ShieldGenerator() = default;
            ShieldGenerator( const ptree& props );

            virtual ~ShieldGenerator() = default;

            virtual void onStep() override;

            virtual ShieldGenerator* clone() const override;

            float shield() const;
            float max_shield() const;

            float recharge_rate() const;

            ShieldGenerator& setShield( float s );

            float getDeflectionChance(const Vec& displacement, const Vec& impact_impulse) const;

        private:
            // properties
            float mMaximumShieldStrength = 1.5;
            float mShieldRecharge        = 0.2;
            float mDecayPerSecond        = 0.1; // per point
            float mEnergyPerShieldPoint  = 5;

            // status
            float mCurrentShieldStrength = 0;
        };
    }
}


#endif //SOI_SHIELDGENERATOR_H
