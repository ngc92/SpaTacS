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
    namespace signatures
    {
        using TankInfoSignature    = core::ecs::Signature<const FuelStorage>;
        using ModifyFuelSignature  = core::ecs::Signature<FuelStorage>;
    }

    class TankInfo : public core::ecs::System<signatures::TankInfoSignature>
    {
    public:
        TankInfo() = default;
        void operator()(const FuelStorage& h);
        mass_t fuel() const     { return mFuel; }
        mass_t capacity() const { return mCapacity; }
    private:
        mass_t mFuel     = 0.0_kg;
        mass_t mCapacity = 0.0_kg;
    };

    class FuelRequest : public core::ecs::System<signatures::ModifyFuelSignature>
    {
    public:
        FuelRequest(mass_t request);
        mass_t fuel() const;

        void operator()(FuelStorage& t);
    private:
        mass_t mRequest = 0.0_kg;
        mass_t mFuel    = 0.0_kg;
    };

    class AddFuel : public core::ecs::System<signatures::ModifyFuelSignature>
    {
    public:
        AddFuel(mass_t amount);
        void operator()(FuelStorage& t);

        mass_t fuel() const;
    private:
        mass_t mFuel;
    };
}
}
}

#endif //SPATACS_GAME_TANKINFO_H
