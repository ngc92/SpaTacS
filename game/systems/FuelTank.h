//
// Created by erik on 12/3/16.
//

#ifndef SPATACS_GAME_TANKINFO_H
#define SPATACS_GAME_TANKINFO_H

#include "fwd.h"

namespace spatacs
{
namespace game
{
namespace systems
{
    class FuelTank : public core::ecs::System<signatures::TankInfoSignature>
    {
    public:
        FuelTank() = default;
        void operator()(const FuelStorage& h);
        mass_t fuel() const     { return mFuel; }
        mass_t capacity() const { return mCapacity; }
    private:
        mass_t mFuel     = 0.0_kg;
        mass_t mCapacity = 0.0_kg;
    };

    /// Requests \p amount fuel. Returns the amount that could be supplied.
    mass_t request_fuel(SubsystemManager& mgr, mass_t amount);

    /// Puts \p amount fuel into the tanks. Returns the amount of fuel that did not fit into
    /// the tanks.
    mass_t fill_fuel(SubsystemManager& mgr, mass_t amount);
}
}
}

#endif //SPATACS_GAME_TANKINFO_H
