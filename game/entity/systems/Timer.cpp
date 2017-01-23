//
// Created by erik on 12/3/16.
//

#include "Timer.h"
#include "../components.h"

using namespace spatacs;
using namespace game::systems;

void TimerCountdown::operator()(components::Timer& timer, time_t dt) const
{
    timer.time -= dt;
}