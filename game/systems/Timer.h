//
// Created by erik on 12/3/16.
//

#ifndef SPATACS_GAME_TIMER_H
#define SPATACS_GAME_TIMER_H

#include "fwd.h"

namespace spatacs
{
namespace game
{
namespace systems
{
    // General purpose components
    struct TimerCountdown : public core::ecs::System<signatures::CountdownSignature>
    {
        void operator()(Timer& timer, time_t dt) const;
    };
}
}
}

#endif //SPATACS_GAME_TIMER_H
