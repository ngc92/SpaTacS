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
}
}
}

#endif //SPATACS_GAME_ENTITY_SUBCOMPONENTS_H
