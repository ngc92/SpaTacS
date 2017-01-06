//
// Created by erik on 1/6/17.
//

#include "Fuel.h"
#include "../subcomponents.h"
#include "core/ecs/EntityManager.h"

using namespace spatacs;
using namespace game;
using namespace systems;

namespace
{
    using components::FuelStorage;

    struct FuelTank : public System<signatures::FuelTank>
    {
        void operator()(const FuelStorage& h, TankInfo& info) const;
    };

    struct FuelRequest : public System<signatures::FuelTank>
    {
        void operator()(FuelStorage& t, mass_t& requested, mass_t& provided) const;
    };


    struct AddFuel : public System<signatures::FuelTank>
    {
        void operator()(FuelStorage& t, mass_t& amount) const;
    };

    void FuelRequest::operator()(FuelStorage& t, mass_t& requested, mass_t& provided) const
    {
        auto get = t.request(requested);
        requested -= get;
        provided += get;
    }

    void AddFuel::operator()(FuelStorage& t, mass_t& amount) const
    {
        amount = t.fill(amount);
    }

    void FuelTank::operator()(const FuelStorage& h, TankInfo& info) const
    {
        info.fuel     += h.current;
        info.capacity += h.capacity;
    }
}

mass_t spatacs::game::systems::request_fuel(game::SubEntityManager& mgr, mass_t amount)
{
    mass_t provided = 0.0_kg;
    mgr.apply(FuelRequest{}, amount, provided);
    return provided;
}

mass_t spatacs::game::systems::fill_fuel(game::SubEntityManager& mgr, mass_t amount)
{
    mgr.apply(AddFuel{}, amount);
    return amount;
}

TankInfo spatacs::game::systems::get_tank_info(const game::SubEntityManager& mgr)
{
    TankInfo info;
    mgr.apply(FuelTank{}, info);
    return info;
}