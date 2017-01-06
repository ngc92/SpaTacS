//
// Created by erik on 1/6/17.
//

#ifndef SPATACS_GAME_ENTITY_SUB_FUEL_H
#define SPATACS_GAME_ENTITY_SUB_FUEL_H

#include "../fwd.h"

namespace spatacs
{
namespace game
{
namespace systems
{
    struct TankInfo
    {
        mass_t fuel     = 0.0_kg;
        mass_t capacity = 0.0_kg;
    };

    /// gets status information for the fuel subsystem
    TankInfo get_tank_info(const SubEntityManager& mgr);

    /// Requests \p amount fuel. Returns the amount that could be supplied.
    mass_t request_fuel(SubEntityManager& mgr, mass_t amount);

    /// Puts \p amount fuel into the tanks. Returns the amount of fuel that did not fit into
    /// the tanks.
    mass_t fill_fuel(SubEntityManager& mgr, mass_t amount);
}
}
}

#endif //SPATACS_GAME_ENTITY_SUB_FUEL_H
