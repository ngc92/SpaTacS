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
    }


    using GameEntityID = TaggedID<std::uint64_t, struct GameEntityTag>;

    namespace detail
    {
        using namespace components;
        using GameEntityConfig = core::ecs::Config<GameEntityID, Name, Timer, Health, Activity, PhysicsData, Parent,
                Warhead, Affiliation, Armour>;
    }

    using GameEntityManager = core::EntityManager<detail::GameEntityConfig>;
    using GameEntityHandle  = core::EntityHandle<detail::GameEntityConfig>;

    namespace systems
    {
        namespace signatures
        {
            using namespace components;
            using ApplyPropulsion = core::ecs::Signature<const PhysicsData, const PropulsionControl, physics::PhysicsWorld>;
        }
    }
}
}

#endif //SPATACS_GAME_ENTITY_FWD_H
