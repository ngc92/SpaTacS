//
// Created by erik on 1/6/17.
//

#ifndef SPATACS_GAME_ENTITY_SUB_POWER_H
#define SPATACS_GAME_ENTITY_SUB_POWER_H

#include "../fwd.h"
#include "physics/units.h"

namespace spatacs
{
namespace game
{
namespace systems
{
    using namespace components;

    class PowerProduction : public System<signatures::PowerPlant>
    {
    public:
        PowerProduction() = default;
        void operator()(PowerPlantData& engine, const Health& health, const Activity& acc, FuelDistributor&);

        const energy_t& getProduced() const { return mProduced; }

    private:
        energy_t mProduced;
    };
}
}
}

#endif //SPATACS_GAME_ENTITY_SUB_POWER_H
