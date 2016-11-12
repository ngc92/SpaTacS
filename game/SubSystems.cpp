//
// Created by erik on 10/7/16.
//

#include "Starship.h"
#include "GameObject.h"
#include "SubSystems.h"
#include "components.h"
#include "systems.h"
#include <boost/property_tree/ptree.hpp>
#include <iostream>
#include "core/System.h"

using namespace spatacs;
using namespace game;

// ---------------------------------------------------------------------------------------------------

SubSystems::SubSystems(const boost::property_tree::ptree& data)
{
    for(auto& child : data)
    {
        if(child.first == "weapon")
            makeProjectileWpn(child.second, mComponents.addEntity());
        if(child.first == "tank")
            makeFuelTank(child.second, mComponents.addEntity());
        if(child.first == "life_support")
            makeLifeSupport(child.second, mComponents.addEntity());
        if(child.first == "power_plant")
            makePowerPlant(child.second, mComponents.addEntity());
        if(child.first == "shieldgen")
            makeShieldGenerator(child.second, mComponents.addEntity());
        if(child.first == "engine")
            makeEngine(child.second, mComponents.addEntity());
        if(child.first == "ammo_storage")
            makeAmmoStorage(child.second, mComponents.addEntity());
    }
}


void SubSystems::onStep(Starship& ship)
{
    mEnergyMgr.process(mComponents);

    TimerCountdown tc(0.1);
    ShieldManagement smgm(ship, mEnergyMgr);
    LifeSupportStep ls(ship, mEnergyMgr);
    Propulsion prop(ship, ship.getDesiredAcceleration());
    TankInfo tank;

    mComponents.apply(tc);
    mComponents.apply(smgm);
    mComponents.apply(ls);
    mComponents.apply(prop);
    mComponents.apply(tank);

    ship.setProducedAcceleration( prop.getProduced() );
    ship.setMaxAcceleration( prop.getMax() );
    ship.setFuelMass(tank.fuel());
}


void EnergyManager::process(core::EntityManager<ComponentEntity>& comps)
{
    PowerProduction gpe(comps);
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
