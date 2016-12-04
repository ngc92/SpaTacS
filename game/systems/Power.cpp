//
// Created by erik on 12/3/16.
//

#include "Power.h"
#include "TankInfo.h"
#include "../components.h"
#include "game/SubSystems.h"

using namespace spatacs;
using namespace game::systems;

PowerProduction::PowerProduction(SubsystemManager& entities) : mEntities(entities)
{

}

void PowerProduction::operator()(const PowerPlantData& pp, const Health& health, const Activity& acc)
{
    auto base_energy = 0.1_s * pp.energy_production * acc.get();
    mass_t need_fuel = base_energy * 10.0_kg / 1.0_MJ;

    FuelRequest request{need_fuel};
    mEntities.apply(request);
    mass_t fuel = request.fuel();

    if(need_fuel < 0.001_kg) need_fuel = 0.001_kg;
    mProducedEnergy += base_energy * health.status() * fuel / need_fuel;
}