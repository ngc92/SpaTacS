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

namespace
{
    class GetProvidedEnergy : public core::System<ComponentEntity, GetProvidedEnergy,
                                                  core::Signature<EnergyManagement>>
    {
    public:
        GetProvidedEnergy() : mCollectedEnergy(0) { }

        void apply(const ComponentEntity& ety, EnergyManagement& egy)
        {
            mCollectedEnergy += egy.cache;
            egy.cache = 0;
        }

        double energy() const { return mCollectedEnergy; }
    private:
        double mCollectedEnergy;
    };

    class CollectRequests : public core::System<ComponentEntity, CollectRequests,
            core::Signature<EnergyManagement>>
    {
    public:
        void apply(const ComponentEntity& ety, EnergyManagement& egy)
        {
            mRequests.push_back( egy.last_request );
            mPriority.push_back( egy.priority );
        }

        const std::vector<double>& requests() { return mRequests; }
        const std::vector<double>& priority() { return mPriority; }

    private:
        std::vector<double> mRequests;
        std::vector<double> mPriority;
    };

    class ProvideEnergy : public core::System<ComponentEntity, ProvideEnergy,
            core::Signature<EnergyManagement>>
    {
    public:
        ProvideEnergy(std::vector<double>& s) : mSupply(s) { }

        void apply(const ComponentEntity& ety, EnergyManagement& egy)
        {
            egy.cache = mSupply.at(i);
            ++i;
        }

    private:
        unsigned i = 0;
        std::vector<double>& mSupply;
    };
}

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
    CollectRequests crq;
    mComponents.apply(crq);

    double energy_to_distribute = energy;

    auto requests = crq.requests();
    // energy distribution according to demand and priority
    std::vector<double> supply(requests.size(), 0.f );

    const int num_iters = 10;
    for(unsigned p = 0; p < num_iters; ++p)
    {
        double total_request = accumulate( begin(requests), end(requests), 0.0 );
        if(total_request == 0)
            break;

        double edist = (p+1) / num_iters * energy_to_distribute;
        for(unsigned i = 0; i < supply.size(); ++i)
        {
            if(requests[i] == 0) continue;

            double add = requests[i] * edist / total_request * crq.priority()[i];

            if(supply[i] + add > crq.requests()[i]) {
               add = crq.requests()[i] - supply[i];
            }
            if(supply[i] + add > energy_to_distribute)
                add = energy_to_distribute - supply[i];

            supply[i] += add;
            requests[i] -= add;
            energy_to_distribute -= add;
        }
    }

    ProvideEnergy pv(supply);
    // supply energy to systems
    mComponents.apply(pv);
    return accumulate(begin(supply), end(supply), 0.0);
}

double SubSystems::produceEnergy()
{
    GetProvidedEnergy gpe;
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
    mComponents.apply(tc);
    mComponents.apply(smgm);
    mComponents.apply(ls);
    mComponents.apply(pp);
    mComponents.apply(prop);

    ship.setProducedAcceleration( prop.getProduced() );
    ship.setMaxAcceleration( prop.getMax() );
}
