//
// Created by erik on 9/18/16.
//

#include "Accelerate.h"
#include "game/Starship.h"
#include <iostream>
#include "game/components/Engine.h"
#include "physics/Events.h"

namespace spatacs
{
    namespace events
    {
        Accelerate::Accelerate(std::uint64_t object, const accel_vec& accel) : ShipEvent( object ),
                                                             mAccel( accel )
        {
        }

        void Accelerate::applyToShip(Starship& target, EventContext& context) const
        {
            // dead ships don't move
            if(!target.alive())
                return;

            /// \todo specify the time here!
            physics::events::ApplyForce af{target.physics_id(),
                                           target.getEngine().getThrust(mAccel * target.mass()),
                                           0.0_s
            };

            context.world.pushEvent(af);
        }
    }
}