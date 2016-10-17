//
// Created by erik on 9/30/16.
//

#ifndef SOI_POWERPLANT_H
#define SOI_POWERPLANT_H

#include "game/components/IComponent.h"

namespace spatacs {
    namespace game {
        class PowerPlant : public IComponent
        {
        public:
            PowerPlant(const ptree& data);

            PowerPlant() = default;
            PowerPlant* clone() const override;

        };
    }
}


#endif //SOI_POWERPLANT_H
