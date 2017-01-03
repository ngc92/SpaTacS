//
// Created by erik on 1/3/17.
//

#ifndef SPATACS_GAME_COMMANDS_H
#define SPATACS_GAME_COMMANDS_H

#include <boost/variant.hpp>
#include "Spawn.h"

namespace spatacs
{
namespace game
{
namespace events
{
    using commands_t = boost::variant<SpawnProjectile, SpawnShip>;
}
}
}

#endif //SPATACS_GAME_COMMANDS_H
