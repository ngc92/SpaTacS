//
// Created by erik on 11/23/16.
//

#ifndef SPATACS_GAME_EVENTS_H
#define SPATACS_GAME_EVENTS_H

#include <boost/variant.hpp>
#include "game/ObjectID.h"

namespace spatacs
{
    namespace game
    {
        namespace events
        {
            /*! The internal event system:
             *  
             */

            // Outgoing Events / Notifications
            struct ReceiveDamage
            {
                ReceiveDamage(ObjectID id_, double damage_) : id(id_), damage(damage_) { }
                ObjectID id;
                double damage;
            };

            struct ShieldAbsorbtion
            {
                ShieldAbsorbtion(ObjectID id_, double absorbed_) : id(id_), absorbed(absorbed_) { }
                ObjectID id;
                double absorbed;
            };

            using notification_t = boost::variant<ReceiveDamage, ShieldAbsorbtion>;
        }
    }
}

#endif //SPATACS_GAME_EVENTS_H
