//
// Created by erik on 12/3/16.
//

#ifndef SPATACS_GAME_POWER_H
#define SPATACS_GAME_POWER_H

#include "fwd.h"

namespace spatacs
{
namespace game
{
    namespace systems
    {
        class PowerProduction : public core::ecs::System<signatures::PowerProductionSignature>
        {
        public:
            PowerProduction() = default;
            void operator()(const PowerPlantData& pp, const Health& health, const Activity& acc, SubsystemManager& entities);
            energy_t energy() const { return mProducedEnergy; }
        private:;
            energy_t mProducedEnergy = 0.0_J;
        };
    }
}
}

#endif //SPATACS_GAME_POWER_H
