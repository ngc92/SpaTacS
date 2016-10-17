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

            virtual ShieldGenerator* clone() const override;

            rate_t<scalar_t> recharge_rate() const;
        };
    }
}


#endif //SOI_SHIELDGENERATOR_H
