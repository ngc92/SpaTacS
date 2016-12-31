//
// Created by erik on 12/3/16.
//

#ifndef SPATACS_GAME_SYSTEMS__SHIELD_H
#define SPATACS_GAME_SYSTEMS__SHIELD_H

#include "fwd.h"

namespace spatacs
{
namespace game
{
    class Shield;
namespace systems
{
    struct ShieldManagement : public core::ecs::System<signatures::ShieldSignature>
    {
        void operator()(ShieldGeneratorData& sgen, const Health& health, const Activity& acc, Shield& shield, EnergyManager& emgr);
    };
}
}
}

#endif //SPATACS_GAME_SYSTEMS__SHIELD_H
