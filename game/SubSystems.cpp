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
    makeEngine(data.get_child("engine"), mComponents.addEntity());
    makeShieldGenerator(data.get_child("shieldgen"), mComponents.addEntity());
    makePowerPlant(data.get_child("power_plant"), mComponents.addEntity());
    makeFuelTank(data.get_child("tank"), mComponents.addEntity());
    makeLifeSupport(data.get_child("life_support"), mComponents.addEntity());
    makeProjectileWpn(data.get_child("weapon"), mComponents.addEntity());

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

    if(mRequested == 0)
        mRequested = 1;
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
