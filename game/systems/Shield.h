//
// Created by erik on 12/3/16.
//

#ifndef SPATACS_GAME_SHIELD_H
#define SPATACS_GAME_SHIELD_H

#include "fwd.h"

namespace spatacs
{
namespace game
{
namespace systems
{
    struct ShieldManagement : public core::ecs::System<signatures::ShieldSignature>
    {
        void operator()(ShieldGeneratorData& sgen, const Health& health, const Activity& acc, Starship& s, EnergyManager& emgr);
    };
}
}
}

#endif //SPATACS_GAME_SHIELD_H
