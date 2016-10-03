//
// Created by erik on 9/18/16.
//

#include "ShipEvent.h"

#include "core/Starship.h"
#include "core/GameState.h"

namespace spatacs
{
    namespace events
    {
        void ShipEvent::apply(EventContext& context) const
        {
            applyToShip(context.state.getShip(id()), context);
        }

        std::uint64_t ShipEvent::id() const
        {
            return mObject;
        }
    }
}