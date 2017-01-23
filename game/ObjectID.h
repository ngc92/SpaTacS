//
// Created by erik on 1/21/17.
//

#ifndef SPATACS_GAME_OBJECTID_H
#define SPATACS_GAME_OBJECTID_H

#include <cstdint>
#include "id.h"

namespace spatacs
{
    namespace game
    {
        using ObjectID = TaggedID<std::uint64_t, struct GameEntityTag>;
    }
}
#endif //SPATACS_GAME_OBJECTID_H
