//
// Created by erik on 12/31/16.
//

#ifndef SPATACS_GAME_ENTITY_FWD_H
#define SPATACS_GAME_ENTITY_FWD_H

#include <cstdint>
#include <game/entity/systems/ShipSystems.h>
#include "core/ecs/System.h"
#include "core/ecs/config.h"
#include "game/ObjectID.h"

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
        // components
        class Timer;
        class Name;
        class Health;
        class Activity;
        class PhysicsData;
        class Parent;
        class Warhead;
        class Affiliation;
        class Armour;
        class PropulsionControl;

        // Ship System components
        class Engine;
        class FuelTank;

        constexpr auto components_list = core::pack_v<Timer, Name, Health, Activity, PhysicsData, Parent, Warhead,
                Affiliation, Armour, PropulsionControl, Engine, FuelTank>;

        // tags
        class TimedDespawn;

        constexpr auto entity_tags = core::pack_v<TimedDespawn>;
    }

    using GameEntityID = ObjectID;

    namespace detail
    {
        constexpr auto GameEntitySetup = core::ecs::make_setup(core::type_v<GameEntityID>,
                                                               components::components_list,
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
            namespace detail
            {
                using namespace components;
                using core::ecs::Signature;

                // put all into a sub-namespace so that we can later do a using declaration without getting also the components namespace
                namespace sigs
                {
                    using ApplyPropulsion    = Signature<PhysicsData, PropulsionControl, Engine, FuelTank>;
                    using UpdatePhysics      = Signature<PhysicsData>;
                    using CountdownSignature = Signature<Timer>;
                }
            }

            using namespace detail::sigs;
        }
    }
}
}

#endif //SPATACS_GAME_ENTITY_FWD_H
