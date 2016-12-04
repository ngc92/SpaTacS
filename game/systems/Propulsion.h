//
// Created by erik on 12/3/16.
//

#ifndef SPATACS_GAME_PROPULSION_H
#define SPATACS_GAME_PROPULSION_H

#include "fwd.h"

namespace spatacs
{
namespace game
{
namespace systems
{
    namespace signatures
    {
        using PropulsionSignature = core::ecs::Signature<EngineData, const Health, const Activity>;
    }

    class Propulsion : public core::ecs::System<signatures::PropulsionSignature>
    {
    public:
        Propulsion(Starship& ship, accel_vec mDesiredAcceleration);
        void operator()(EngineData& engine, const Health& health, const Activity& acc);

        const accel_vec& getProduced() const { return mProducedAcceleration; }
        accel_t getMax() const { return mMaxAcceleration; }

    private:
        Starship& mShip;
        accel_vec mDesiredAcceleration;
        accel_vec mProducedAcceleration{.0, .0, .0};
        accel_t   mMaxAcceleration{.0};
    };
}
}
}

#endif //SPATACS_GAME_PROPULSION_H
