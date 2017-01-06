//
// Created by erik on 1/4/17.
//

#ifndef SPATACS_GAME_EVENTS_CONTROLS_H
#define SPATACS_GAME_EVENTS_CONTROLS_H

#include "physics/units.h"
#include "context.h"
#include "game/entity/fwd.h"

namespace spatacs
{
namespace game
{
namespace events
{
    // Flight control
    class SetDesiredAcceleration
    {
    public:
        SetDesiredAcceleration(GameEntityID target, const accel_vec& av);
        void apply(EventContext& context) const;
    private:
        GameEntityID  mTarget;
        accel_vec     mDesiredAccel;
    };
}
}
}

#endif //SPATACS_GAME_EVENTS_CONTROLS_H
