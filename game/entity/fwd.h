//
// Created by erik on 12/31/16.
//

#ifndef SPATACS_GAME_ENTITY_FWD_H
#define SPATACS_GAME_ENTITY_FWD_H

#include <cstdint>
#include "id.h"
#include "core/ecs/fwd.h"
#include "core/ecs/System.h"

namespace spatacs
{
namespace physics
{
    class PhysicsWorld;
}
namespace game
{
    namespace components
    {
        // common components
        class Timer;
        class Name;
        class Health;
        class Activity;

        // GameEntity components
        class PhysicsData;
        class Parent;
        class Warhead;
        class Affiliation;
        class Armour;
        class PropulsionControl;
        class SubEntities;
    }

    using GameEntityID = TaggedID<std::uint64_t, struct GameEntityTag>;

    namespace detail
    {
        using namespace components;
        using GameEntityConfig = core::ecs::Config<GameEntityID, Name, Timer, Health, Activity, PhysicsData, Parent,
                Warhead, Affiliation, Armour, SubEntities>;
    }

    using GameEntityManager = core::EntityManager<detail::GameEntityConfig>;
    using GameEntityHandle  = core::EntityHandle<detail::GameEntityConfig>;

    namespace systems
    {
        using core::ecs::System;
        namespace signatures
        {
            namespace c = components;
            using core::ecs::Signature;

            using ApplyPropulsion = Signature<c::PhysicsData, c::PropulsionControl, c::SubEntities>;

            using UpdatePhysics   = Signature<c::PhysicsData>;
        }
    }
}
}

#endif //SPATACS_GAME_ENTITY_FWD_H
