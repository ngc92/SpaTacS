#ifndef EVENTS_SHIP_EVENT_H_INCLUDED
#define EVENTS_SHIP_EVENT_H_INCLUDED

#include <cstdint>
#include "IEvent.h"

namespace spatacs
{
namespace game
{
    class Starship;
}

namespace events
{
    class ShipEvent : public IEvent
    {
    public:
        using Starship  = game::Starship;

        ShipEvent( std::uint64_t target ) : mObject( target ) { }

        std::uint64_t id() const;

        /// applies this event to the target.
        /// The actual application is handled through the
        /// virtual \p applyToShip function.
        void apply(EventContext& context) const override;
    private:

        virtual void applyToShip(Starship& target, EventContext& context) const = 0;

        std::uint64_t mObject;
    };
}
}

#endif