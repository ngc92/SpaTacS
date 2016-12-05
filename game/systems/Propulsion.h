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
    class Propulsion : public core::ecs::System<signatures::PropulsionSignature>
    {
    public:
        Propulsion() = default;
        void operator()(EngineData& engine, const Health& health, const Activity& acc,
                        Starship& ship, const accel_vec& desired_acceleration);

        const accel_vec& getProduced() const { return mProducedAcceleration; }
        accel_t getMax() const { return mMaxAcceleration; }

    private:
        accel_vec mProducedAcceleration{.0, .0, .0};
        accel_t   mMaxAcceleration{.0};
    };
}
}
}

#endif //SPATACS_GAME_PROPULSION_H
