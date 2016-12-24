//
// Created by erik on 11/1/16.
//

#ifndef SPATACS_PHYSICS_OBJECTID_H
#define SPATACS_PHYSICS_OBJECTID_H

#include "id.h"
#include <cstdint>

namespace spatacs
{
    namespace physics
    {
        using ObjectID  = TaggedID<std::uint64_t, struct object_tag>;
        using FixtureID = TaggedID<std::uint64_t, struct fixture_tag>;
    }
}

#endif //SPATACS_OBJECTID_H
