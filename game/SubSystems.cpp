//
// Created by erik on 10/7/16.
//

#include "Starship.h"
#include "GameObject.h"
#include "SubSystems.h"
#include "components.h"
#include "systems/Timer.h"
#include "game/systems/FuelTank.h"
#include "systems/Propulsion.h"
#include "systems/Shield.h"
#include "systems/Power.h"
#include "systems/LifeSupport.h"
#include <boost/property_tree/ptree.hpp>
#include <iostream>
#include "core/ecs/EntityHandle.h"

using namespace spatacs;
using namespace game;

// ---------------------------------------------------------------------------------------------------

SubSystems::SubSystems(const boost::property_tree::ptree& data)
{
    for(auto& child : data)
    {
        if(child.first == "weapon")
            makeProjectileWpn(child.second, mComponents.create());
        if(child.first == "tank")
            makeFuelTank(child.second, mComponents.create());
        if(child.first == "life_support")
            makeLifeSupport(child.second, mComponents.create());
        if(child.first == "power_plant")
            makePowerPlant(child.second, mComponents.create());
        if(child.first == "shieldgen")
            makeShieldGenerator(child.second, mComponents.create());
        if(child.first == "engine")
            makeEngine(child.second, mComponents.create());
        if(child.first == "ammo_storage")
            makeAmmoStorage(child.second, mComponents.create());
    }
}


void SubSystems::onStep(Starship& ship)
{
    mEnergyMgr.process(mComponents);

    systems::TimerCountdown tc(0.1);
    systems::ShieldManagement smgm(ship, mEnergyMgr);
    systems::LifeSupportStep ls(ship, mEnergyMgr);
    systems::Propulsion prop(ship, ship.getDesiredAcceleration());
    systems::FuelTank tank;

    mComponents.apply(tc);
    mComponents.apply(smgm);
    mComponents.apply(ls);
    mComponents.apply(prop);
    mComponents.apply(tank);

    ship.setProducedAcceleration( prop.getProduced() );
    ship.setMaxAcceleration( prop.getMax() );
    ship.setFuelMass(tank.fuel());
}

double SubSystems::dealDamage(double dmg)
{
    auto ids = mComponents.id_range();
    auto dmg_target = rand() % size(ids);
    auto iter = begin(ids);
    std::advance(iter, dmg_target);

    auto cmp = mComponents.get(*iter);

    // one shot can only ever destroy half the component
    if(cmp.has<Health>()) {
        double d = std::min(dmg, cmp.get<Health>().current / 2);
        cmp.get<Health>().current -= d;
        dmg -= d;
    }
    return dmg;
}

void EnergyManager::process(SubsystemManager& comps)
{
    systems::PowerProduction gpe(comps);
    comps.apply(gpe);
    mTotal = gpe.energy();

    if(mRequested < 0.1_J)
        mRequested = 0.1_J;
    mSupplyFactor = std::min(1.0, (double)(mTotal / mRequested));

    mPowerLeft = mTotal;
    mRequested = 0.0_J;
}

energy_t EnergyManager::requestPower(energy_t amount)
{
    mRequested += amount;
    energy_t scaled = amount * mSupplyFactor;
    energy_t real = std::min(mPowerLeft, scaled);
    mPowerLeft -= real;
    return real;
}
