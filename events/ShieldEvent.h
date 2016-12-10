//
// Created by erik on 12/10/16.
//

#ifndef SPATACS_SHIELDEVENT_H
#define SPATACS_SHIELDEVENT_H

#include "IEvent.h"
#include "game/ObjectID.h"

namespace spatacs
{
    namespace events
    {
        class ShieldEvent : public IEvent
        {
        public:
            ShieldEvent(game::ObjectID target, double change);
            /// applies this event to the game state.
            void apply(EventContext& context) const override;
        private:
            game::ObjectID mShieldObject;
            double mChange;
        };
    }
}

#endif //SPATACS_SHIELDEVENT_H
