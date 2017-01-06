//
// Created by erik on 1/6/17.
//

#ifndef SPATACS_GAME_ENTITY_SUBSYS_H
#define SPATACS_GAME_ENTITY_SUBSYS_H

#include "../fwd.h"

namespace spatacs
{
namespace game
{
namespace systems
{
    class SubCompSystem : signatures::SubComonents
    {
    public:
        void operator()(components::SubEntities& subs);
    };
}
}
}

#endif //SPATACS_GAME_ENTITY_SUBSYS_H
