//
// Created by erik on 1/3/17.
//

#ifndef SPATACS_GAME_ENTITY_SUB_FWD_H
#define SPATACS_GAME_ENTITY_SUB_FWD_H

#include <cstdint>
#include "id.h"
#include "core/ecs/fwd.h"
#include "../fwd.h"

namespace spatacs
{
namespace game
{
    namespace components
    {
        class EngineData;
    }

    using SubEntityID = TaggedID<std::uint64_t, struct SubEntityTag>;

    namespace detail
    {
        using namespace components;
        using SubEntityConfig = core::ecs::Config<SubEntityID, Name, Timer, Health, Activity, EngineData>;
    }

    using SubEntityManager = core::EntityManager<detail::SubEntityConfig>;
    using SubEntityHandle  = core::EntityHandle<detail::SubEntityConfig>;

    namespace systems
    {
        using core::ecs::System;
        namespace signatures
        {
            namespace c = components;
            using core::ecs::Signature;

            using Engine = Signature<c::EngineData, const c::Health, const c::Activity>;
        }
    }
}
}

#endif //SPATACS_GAME_ENTITY_SUB_FWD_H
