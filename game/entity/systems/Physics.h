//
// Created by erik on 1/3/17.
//

#ifndef SPATACS_GAME_ENTITY_PHYSICS_H
#define SPATACS_GAME_ENTITY_PHYSICS_H

#include "../fwd.h"

namespace spatacs
{
    namespace game
    {
        namespace systems
        {
            using namespace components;
            using core::ecs::System;
            struct UpdateFromWorld : public System<signatures::UpdatePhysics >
            {
                void operator()(PhysicsData&, const physics::PhysicsWorld&) const;
            };

            struct UpdateToWorld : public System<signatures::UpdatePhysics >
            {
                void operator()(const PhysicsData&, physics::PhysicsWorld&) const;
            };
        }
    }
}


#endif //SPATACS_GAME_ENTITY_PHYSICS_H
