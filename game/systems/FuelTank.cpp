//
// Created by erik on 12/3/16.
//

#include "FuelTank.h"
#include "../components.h"
#include "core/ecs/EntityManager.h"

using namespace spatacs;
using namespace game::systems;

namespace {
    struct FuelTank : public core::ecs::System<signatures::TankInfoSignature>
    {
        void operator()(const game::FuelStorage& h, TankInfo& info) const;
    };

    struct FuelRequest : public core::ecs::System<signatures::ModifyFuelSignature>
    {
        void operator()(game::FuelStorage& t, mass_t& requested, mass_t& provided) const;
    };


    struct AddFuel : public core::ecs::System<signatures::ModifyFuelSignature>
    {
        void operator()(game::FuelStorage& t, mass_t& amount) const;
    };

    void FuelRequest::operator()(game::FuelStorage& t, mass_t& requested, mass_t& provided) const
    {
        auto get = t.request(requested);
        requested -= get;
        provided += get;
    }

    void AddFuel::operator()(game::FuelStorage& t, mass_t& amount) const
    {
        amount = t.fill(amount);
    }

    void FuelTank::operator()(const game::FuelStorage& h, TankInfo& info) const
    {
        info.fuel     += h.current;
        info.capacity += h.capacity;
    }
}

mass_t spatacs::game::systems::request_fuel(game::SubsystemManager& mgr, mass_t amount)
{
    mass_t provided = 0.0_kg;
    mgr.apply(FuelRequest{}, amount, provided);
    return provided;
}

mass_t spatacs::game::systems::fill_fuel(game::SubsystemManager& mgr, mass_t amount)
{
    mgr.apply(AddFuel{}, amount);
    return amount;
}

TankInfo spatacs::game::systems::get_tank_info(const game::SubsystemManager& mgr)
{
    TankInfo info;
    mgr.apply(FuelTank{}, info);
    return info;
}