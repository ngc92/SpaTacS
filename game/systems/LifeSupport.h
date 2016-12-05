//
// Created by erik on 12/3/16.
//

#ifndef SPATACS_GAME_LIFESUPPORT_H
#define SPATACS_GAME_LIFESUPPORT_H

#include "fwd.h"

namespace spatacs
{
namespace game
{
namespace systems
{
    struct LifeSupportStep : public core::ecs::System<signatures::LifeSupportSignature>
    {
        void operator()(LifeSupportData& sup, const Starship& ship, EnergyManager& e) const;
    };
}
}
}


#endif //SPATACS_GAME_LIFESUPPORT_H
