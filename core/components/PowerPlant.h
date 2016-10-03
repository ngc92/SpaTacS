//
// Created by erik on 9/30/16.
//

#ifndef SOI_POWERPLANT_H
#define SOI_POWERPLANT_H

#include "IComponent.h"

namespace spatacs {
    namespace core {
        class PowerPlant : public IComponent
        {
        public:
            PowerPlant(const ptree& data);

            void onStep() override;

            PowerPlant() = default;
            PowerPlant* clone() const override;

            float power() const;
        private:
            float mEnergyProduction = 1;
        };
    }
}


#endif //SOI_POWERPLANT_H
