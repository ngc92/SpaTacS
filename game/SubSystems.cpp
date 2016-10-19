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
    GetEnergyRequest rq;
    mComponents.apply(rq);
    double f = std::min(1.0, energy / rq.request());
    ProvideEnergy provide(energy, f);
    mComponents.apply(provide);

}

double SubSystems::produceEnergy()
{
    PowerProduction gpe;
    mComponents.apply(gpe);
    return gpe.energy();
}

void SubSystems::onStep(Starship& ship)
{
    TimerCountdown tc(0.1);
    ShieldManagement smgm(ship);
    LifeSupportStep ls(ship);
    PowerProduction pp;
    Propulsion prop(ship, ship.getDesiredAcceleration());
    TankInfo tank;

    mComponents.apply(tc);
    mComponents.apply(smgm);
    mComponents.apply(ls);
    mComponents.apply(pp);
    mComponents.apply(prop);
    mComponents.apply(tank);

    FuelDistribution fd(tank.fuel());
    mComponents.apply(fd);
    FuelConsumption fc(tank.fuel() - fd.fuel());
    mComponents.apply(fc);

    ship.setProducedAcceleration( prop.getProduced() );
    ship.setMaxAcceleration( prop.getMax() );
    ship.setFuelMass(tank.fuel());
}
