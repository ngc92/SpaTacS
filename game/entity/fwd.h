//
// Created by erik on 12/31/16.
//

#ifndef SPATACS_GAME_ENTITY_FWD_H
#define SPATACS_GAME_ENTITY_FWD_H

#include <cstdint>
#include "id.h"
#include "core/ecs/System.h"
#include "core/ecs/config.h"

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

        constexpr auto common_components = core::pack_v<Timer, Name, Health, Activity>;

        // GameEntity components
        class PhysicsData;
        class Parent;
        class Warhead;
        class Affiliation;
        class Armour;
        class PropulsionControl;
        class SubEntities;

        constexpr auto entity_components = core::pack_v<PhysicsData, Parent, Warhead, Affiliation, Armour,
                PropulsionControl, SubEntities>;

        // tags
        class TimedDespawn;

        constexpr auto entity_tags = core::pack_v<TimedDespawn>;
    }

    using GameEntityID = TaggedID<std::uint64_t, struct GameEntityTag>;

    namespace detail
    {
        constexpr auto GameEntitySetup = core::ecs::make_setup(core::type_v<GameEntityID>,
                                      concat(components::common_components, components::entity_components),
                                                               components::entity_tags);
        using GameEntityConfig = decltype(make_config(GameEntitySetup));
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

            using ApplyPropulsion    = Signature<c::PhysicsData, c::PropulsionControl, c::SubEntities>;
            using SubComonents       = Signature<c::SubEntities>;
            using UpdatePhysics      = Signature<c::PhysicsData>;
            using CountdownSignature = Signature<c::Timer>;
        }
    }
}
}

#endif //SPATACS_GAME_ENTITY_FWD_H
