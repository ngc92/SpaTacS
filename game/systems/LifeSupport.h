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
    namespace signatures
    {
        using LifeSupportSignature = core::ecs::Signature<LifeSupportData>;
    }

    class LifeSupportStep : public core::ecs::System<signatures::LifeSupportSignature>
    {
    public:
        LifeSupportStep(const Starship& s, EnergyManager& e);
        void operator()(LifeSupportData& sup) const;
    private:
        const Starship& ship;
        EnergyManager& emgr;
    };
}
}
}


#endif //SPATACS_GAME_LIFESUPPORT_H
