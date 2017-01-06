//
// Created by erik on 1/3/17.
//

#ifndef SPATACS_GAME_ENTITY_SUBCOMPONENTS_H
#define SPATACS_GAME_ENTITY_SUBCOMPONENTS_H

#include "physics/units.h"
#include "../common_comps.h"

namespace spatacs
{
namespace game
{
namespace components
{
    struct EngineData
    {
        EngineData() = default;
        EngineData(speed_t s, rate_t<mass_t> r) :
                propellant_speed(s), mass_rate(r)
        {

        }

        // engine config
        speed_t propellant_speed = 0.0_kps;
        rate_t<mass_t> mass_rate = 1.0_kg / 1.0_s;
    };

    struct FuelStorage
    {
        FuelStorage() = default;
        explicit FuelStorage(mass_t v);

        mass_t request(mass_t desire);
        mass_t fill(mass_t amount);

        // data about stored fuel
        mass_t current;
        mass_t capacity;
    };

    struct PowerPlantData
    {
        PowerPlantData() = default;
        explicit PowerPlantData(power_t ep) : energy_production(ep) {}
        power_t energy_production = 0.0_W;
    };

    struct ShieldGeneratorData
    {
        ShieldGeneratorData() = default;

        // properties
        rate_t<scalar_t> mShieldRecharge{0.0};
        energy_t         mEnergyPerShieldPoint{1.0};
    };
}
}
}

#endif //SPATACS_GAME_ENTITY_SUBCOMPONENTS_H
