//
// Created by erik on 12/31/16.
//

#ifndef SPATACS_GAME_ENTITY_PROPULSION_H
#define SPATACS_GAME_ENTITY_PROPULSION_H

#include "../fwd.h"

namespace spatacs
{
    namespace game
    {
        namespace systems
        {
            using namespace components;
            struct ApplyPropulsion : public core::ecs::System<signatures::ApplyPropulsion>
            {
                void operator()(const PhysicsData&, const PropulsionControl&, physics::PhysicsWorld&) const;
            };
        }
    }
}

#endif //SPATACS_GAME_ENTITY_PROPULSION_H
