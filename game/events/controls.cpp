//
// Created by erik on 1/4/17.
//

#include "controls.h"
#include "game/State.h"
#include "game/entity/entity.h"

using namespace spatacs;
using namespace game;
using namespace events;

SetDesiredAcceleration::SetDesiredAcceleration(GameEntityID target, const accel_vec& av) :
        mTarget(target), mDesiredAccel(av)
{
}

void SetDesiredAcceleration::apply(EventContext& context) const
{
    struct SDAS : core::ecs::System<core::ecs::Signature<components::PropulsionControl>>
    {
        void operator()(components::PropulsionControl& pc, const accel_vec& tacc)
        {
            pc.setDesiredAcceleration(tacc);
        }
    };
    context.state.entity_manager().apply_to(mTarget, SDAS{}, mDesiredAccel);
}

