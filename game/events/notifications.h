//
// Created by erik on 11/23/16.
//

#ifndef SPATACS_GAME_EVENTS_H
#define SPATACS_GAME_EVENTS_H

#include <boost/variant.hpp>
#include "game/entity/fwd.h"

namespace spatacs
{
namespace game
{
    namespace events
    {
        // Outgoing Events / Notifications
        struct ReceiveDamage
        {
            ReceiveDamage(GameEntityID id_, double damage_) : id(id_), damage(damage_) { }
            GameEntityID id;
            double damage;
        };

        struct ShieldAbsorbtion
        {
            ShieldAbsorbtion(GameEntityID id_, double absorbed_) : id(id_), absorbed(absorbed_) { }
            GameEntityID id;
            double absorbed;
        };

        using notification_t = boost::variant<ReceiveDamage, ShieldAbsorbtion>;

        // Commands

    }
}
}

#endif //SPATACS_GAME_EVENTS_H
