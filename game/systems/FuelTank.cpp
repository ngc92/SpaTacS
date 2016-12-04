//
// Created by erik on 12/3/16.
//

#include "FuelTank.h"
#include "../components.h"
#include "core/ecs/EntityManager.h"

using namespace spatacs;
using namespace game::systems;

void FuelTank::operator()(const FuelStorage& h)
{
    mFuel     += h.current;
    mCapacity += h.capacity;
}


namespace {
    class FuelRequest : public core::ecs::System<signatures::ModifyFuelSignature>
    {
    public:
        FuelRequest(mass_t request);

        mass_t fuel() const;

        void operator()(game::FuelStorage& t);

    private:
        mass_t mRequest = 0.0_kg;
        mass_t mFuel = 0.0_kg;
    };


    class AddFuel : public core::ecs::System<signatures::ModifyFuelSignature>
    {
    public:
        AddFuel(mass_t amount);

        void operator()(game::FuelStorage& t);

        mass_t fuel() const;

    private:
        mass_t mFuel;
    };

    FuelRequest::FuelRequest(mass_t request) :
            mRequest(request)
    {
    }

    mass_t FuelRequest::fuel() const
    {
        return mFuel;
    }

    void FuelRequest::operator()(game::FuelStorage& t)
    {
        auto get = t.request(mRequest);
        mRequest -= get;
        mFuel += get;
    }

    AddFuel::AddFuel(mass_t amount) : mFuel(amount)
    {

    }

    void AddFuel::operator()(game::FuelStorage& t)
    {
        mFuel = t.fill(mFuel);
    }

    mass_t AddFuel::fuel() const
    {
        return mFuel;
    }
}

mass_t spatacs::game::systems::request_fuel(game::SubsystemManager& mgr, mass_t amount)
{
    FuelRequest request{amount};
    mgr.apply(request);
    return request.fuel();
}

mass_t spatacs::game::systems::fill_fuel(game::SubsystemManager& mgr, mass_t amount)
{
    AddFuel adding{amount};
    mgr.apply(adding);
    return adding.fuel();
}

