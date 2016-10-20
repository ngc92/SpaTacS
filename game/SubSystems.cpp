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
    }

    mComponents.apply([this](game::ComponentEntity& cmp)
                      { if(cmp.has<WeaponAimData>()){ mArmament.push_back(&cmp);} });
}

SubSystems::SubSystems( const SubSystems& other ):
        mComponents( other.mComponents )
{
    mComponents.apply([this](game::ComponentEntity& cmp)
                      { if(cmp.has<WeaponAimData>()){ mArmament.push_back(&cmp);} });
}

double SubSystems::distributeEnergy(double energy)
{
    double request = 0;
    double f = std::min(1.0, energy / request);
}

double SubSystems::produceEnergy()
{
    PowerProduction gpe;
    mComponents.apply(gpe);
    return gpe.energy();
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
    PowerProduction gpe;
    comps.apply(gpe);
    mTotal = gpe.energy();

    if(mRequested < 0.1)
        mRequested = 0.1;
    mSupplyFactor = std::min(1.0, mTotal / mRequested);

    mPowerLeft = mTotal;
    mRequested = 0;
}

double EnergyManager::requestPower(double amount)
{
    mRequested += amount;
    double scaled = amount * mSupplyFactor;
    double real = std::min(mPowerLeft, scaled);
    mPowerLeft -= real;
    return real;
}
