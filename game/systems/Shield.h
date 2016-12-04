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
    namespace signatures
    {
        using ShieldSignature = core::ecs::Signature<ShieldGeneratorData, const Health, const Activity>;
    }

    class ShieldManagement : public core::ecs::System<signatures::ShieldSignature>
    {
    public:
        ShieldManagement(Starship& s, EnergyManager& emgr);
        void operator()(ShieldGeneratorData& sgen, const Health& health, const Activity& acc);
    private:
        Starship& ship;
        EnergyManager& emgr;
    };
}
}
}

#endif //SPATACS_GAME_SHIELD_H
