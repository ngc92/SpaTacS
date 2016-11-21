//
// Created by erik on 9/18/16.
//

#include "ShipEvent.h"

#include "game/Starship.h"
#include "game/GameState.h"

namespace spatacs
{
    namespace events
    {
        void ShipEvent::apply(EventContext& context) const
        {
            if(context.state.hasObject(id()) && context.state.getObject(id()).alive()) {
                applyToShip(context.state.getShip(id()), context);
            }
        }

        game::ObjectID ShipEvent::id() const
        {
            return mObject;
        }
    }
}