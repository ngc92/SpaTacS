//
// Created by erik on 9/21/16.
//

#ifndef SOI_IEVENT_H
#define SOI_IEVENT_H

#include <memory>
#include <vector>
#include "physics/units.h"

namespace spatacs
{
    namespace game
    {
        class GameState;
    }

    namespace physics
    {
        class PhysicsWorld;
    }

    namespace events
    {
        class IEvent;

        using EventPtr  = std::unique_ptr<IEvent>;
        using EventList = std::vector<EventPtr>;

        struct EventContext
        {
            game::GameState& state;
            EventList& events;
            physics::PhysicsWorld& world;
        };

        class IEvent
        {
        public:
            using State     = game::GameState;

            /// applies this event to the game state.
            virtual void apply(EventContext& context) const = 0;
        };
    }
}

#endif //SOI_IEVENT_H
