//
// Created by erik on 1/2/17.
//

#ifndef SPATACS_GAME_CONTEXT_H
#define SPATACS_GAME_CONTEXT_H

///! \todo figure out where to put this class: game or game::events.

#include "events.h"

namespace spatacs
{
namespace physics
{
    class PhysicsWorld;
}
namespace game
{
    class State;

    namespace events
    {
        //! \todo turn this into a real class!
        //! \todo tests.
        struct EventContext
        {
            State& state;
            physics::PhysicsWorld& world;
            std::vector<notification_t>& notifications;
        };
    }
}
}

#endif //SPATACS_GAME_CONTEXT_H
