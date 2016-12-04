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
    namespace signatures {
        using CountdownSignature = core::ecs::Signature<Timer>;
    }

    // General purpose components
    class TimerCountdown : public core::ecs::System<signatures::CountdownSignature>
    {
    public:
        TimerCountdown(double dt);
        void operator()(Timer& timer) const;
    private:
        double mDeltaT;
    };
}
}
}

#endif //SPATACS_GAME_TIMER_H
