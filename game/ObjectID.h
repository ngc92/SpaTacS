//
// Created by erik on 11/2/16.
//

#ifndef SPATACS_OBJECTID_H
#define SPATACS_OBJECTID_H

#include <cstdint>
#include "id.h"

namespace spatacs
{
    namespace game
    {
        enum class ObjectType
        {
            STARSHIP,
            PROJECTILE
        };

        using ObjectID = TaggedID<std::uint64_t, struct game_object_tag>;
    }
}
#endif //SPATACS_OBJECTID_H
