//
// Created by erik on 10/7/16.
//

#include "Starship.h"
#include "GameObject.h"
#include "SubSystems.h"
#include <boost/property_tree/ptree.hpp>
#include <iostream>
#include "components/ProjectileWeapon.h"

using namespace spatacs;
using namespace core;

SubSystems::SubSystems(const boost::property_tree::ptree& data):
        mEngine(new Engine(data.get_child("engine"))),
        mShield(new ShieldGenerator(data.get_child("shield"))),
        mPowerPlant(new PowerPlant(data.get_child("power_plant"))),
        mFuelTank(new FuelTank(data.get_child("tank"))),
        mLifeSupport( new LifeSupport(data.get_child("life_support")) )
{
    mArmament.push_back(std::make_unique<ProjectileWeapon>(data.get_child("weapon")));

    mCompPtrs.push_back( mEngine.get() );
    mCompPtrs.push_back( mShield.get() );
    mCompPtrs.push_back( mPowerPlant.get() );
    mCompPtrs.push_back( mLifeSupport.get() );
    for(auto& wpn : mArmament)
        mCompPtrs.push_back( wpn.get() );
}

namespace {
    template<class T>
    std::unique_ptr<T> clone(const std::unique_ptr<T>& original)
    {
        return std::unique_ptr<T>(original->clone());
    }

    template<class T>
    std::vector<T> clone(const std::vector<T>& original)
    {
        std::vector<T> cl;
        cl.reserve(original.size());
        std::transform(begin(original), end(original),
                       std::back_inserter(cl), [](const T& t) { return clone(t); });
        return std::move(cl);
    }
}

SubSystems::SubSystems( const SubSystems& other ):
        mEngine( clone(other.mEngine) ),
        mShield( clone(other.mShield) ),
        mPowerPlant( clone(other.mPowerPlant) ),
        mFuelTank( clone(other.mFuelTank) ),
        mLifeSupport( clone(other.mLifeSupport) ),
        mArmament( clone(other.mArmament) )
{
    mCompPtrs.push_back( mEngine.get() );
    mCompPtrs.push_back( mShield.get() );
    mCompPtrs.push_back( mPowerPlant.get() );
    mCompPtrs.push_back( mLifeSupport.get() );
    for(auto& wpn : mArmament)
        mCompPtrs.push_back( wpn.get() );
}

double SubSystems::distributeEnergy(double energy)
{
    std::vector<double> requests;

    double energy_to_distribute = energy;
    for(auto& sys : mCompPtrs) {
        requests.push_back( sys->getEnergyRequest());
    }

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

            double add = requests[i] * edist / total_request * mCompPtrs[i]->energyPriority();

            if(supply[i] + add > mCompPtrs[i]->getEnergyRequest()) {
               add = mCompPtrs[i]->getEnergyRequest() - supply[i];
            }
            if(supply[i] + add > energy_to_distribute)
                add = energy_to_distribute - supply[i];

            supply[i] += add;
            requests[i] -= add;
            energy_to_distribute -= add;
        }
    }

    // supply energy to systems
    for(unsigned i = 0; i < supply.size(); ++i) {
        mCompPtrs[i]->provideEnergy(supply[i]);
    }
    return accumulate(begin(supply), end(supply), 0.0);;
}

double SubSystems::produceEnergy() const
{
    double energy_to_distribute = 0;
    // get excess energy from each component
    for(auto& sys : mCompPtrs) {
        energy_to_distribute += sys->getExcessEnergy();
    }
    return energy_to_distribute;
}