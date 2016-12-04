//
// Created by erik on 12/3/16.
//

#include "Timer.h"
#include "../components.h"

using namespace spatacs;
using namespace game::systems;

TimerCountdown::TimerCountdown(double dt) : mDeltaT( dt )
{
}

void TimerCountdown::operator()(Timer& timer) const
{
    timer.time -= mDeltaT;
}