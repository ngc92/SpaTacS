//
// Created by erik on 1/6/17.
//

#include "Power.h"
#include "../subcomponents.h"
#include "../managers.h"

using namespace spatacs;
using namespace game::systems;

void PowerProduction::operator()(PowerPlantData& pp, const Health& health, const Activity& acc, FuelDistributor& fuelmgr)
{
    auto base_energy = 0.1_s * pp.energy_production * acc.get();
    mass_t need_fuel = base_energy * 10.0_kg / 1.0_MJ;

    mass_t fuel = fuelmgr.request(need_fuel);

    mProduced += base_energy * health.status() * ratio(fuel, need_fuel);
}