#ifndef SOI_EVENTS_MOVE_H_INCLUDED
#define SOI_EVENTS_MOVE_H_INCLUDED

#include "ShipEvent.h"
#include "physics/units.h"

namespace spatacs
{
namespace events
{
    class Accelerate: public ShipEvent
    {
    public:
        Accelerate(game::ObjectID object, const accel_vec& delta);

    private:
        virtual void applyToShip(Starship& target, EventContext& state) const override;

        accel_vec mAccel;
    };
}
}

#endif
