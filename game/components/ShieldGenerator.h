//
// Created by erik on 9/21/16.
//

#ifndef SOI_SHIELDGENERATOR_H
#define SOI_SHIELDGENERATOR_H

#include "physics/units.h"
#include "game/components/IComponent.h"

namespace spatacs {
    namespace game {
        class ShieldGenerator : public IComponent
        {
        public:
            ShieldGenerator() = default;
            ShieldGenerator( const ptree& props );

            virtual ~ShieldGenerator() = default;

            virtual void onStep(Starship& ship) override;

            virtual ShieldGenerator* clone() const override;

            float shield() const;
            float max_shield() const;

            double recharge_rate() const;
            ShieldGenerator& setShield( float s );

        private:
            // properties
            float mMaximumShieldStrength = 1.5;
            float mShieldRecharge        = 0.2;
            rate_t<scalar_t> mDecay;
            float mEnergyPerShieldPoint  = 5;

            // status
            float mCurrentShieldStrength = 0;
        };
    }
}


#endif //SOI_SHIELDGENERATOR_H