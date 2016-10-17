//
// Created by erik on 9/18/16.
//

#include "Accelerate.h"
#include "game/Starship.h"
#include <iostream>

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
            target.setDesiredAcceleration(mAccel);
        }
    }
}