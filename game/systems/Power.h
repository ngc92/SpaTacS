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
        namespace signatures
        {
            using PowerProductionSignature = core::ecs::Signature<const PowerPlantData, const Health, const Activity>;
        }

        class PowerProduction : public core::ecs::System<signatures::PowerProductionSignature>
        {
        public:
            PowerProduction(SubsystemManager& entities);
            void operator()(const PowerPlantData& pp, const Health& health, const Activity& acc);
            energy_t energy() const { return mProducedEnergy; }
        private:
            SubsystemManager& mEntities;
            energy_t mProducedEnergy = 0.0_J;
        };
    }
}
}

#endif //SPATACS_GAME_POWER_H
