//
// Created by erik on 9/18/16.
//

#include "Starship.h"
#include <cmath>
#include <algorithm>
#include "components/ProjectileWeapon.h"
#include "components/ShieldGenerator.h"
#include "components/Engine.h"
#include "components/Hull.h"
#include <boost/property_tree/ptree.hpp>
#include "components/PowerPlant.h"

using namespace spatacs;
using namespace core;

struct Starship::SubSystems
{
    SubSystems(const boost::property_tree::ptree& data):
        mEngine(new Engine(data.get_child("engine"))),
        mShield(new ShieldGenerator(data.get_child("shield"))),
        mPowerPlant(new PowerPlant(data.get_child("power_plant"))),
        mHull( new Hull(data.get_child("hull")) )
    {
        mArmament.push_back(std::make_unique<ProjectileWeapon>(data.get_child("weapon")));
    }

    SubSystems( const SubSystems& other );

    std::unique_ptr<Engine> mEngine;
    std::unique_ptr<ShieldGenerator> mShield;
    std::unique_ptr<PowerPlant> mPowerPlant;
    std::unique_ptr<Hull> mHull;
    std::vector<std::unique_ptr<IWeapon>> mArmament;
};

ShipData::ShipData(std::uint64_t team, std::string name) : mTeam(team), mName( std::move(name) )
{

}


Starship::Starship()
{

}
Starship::Starship(std::uint64_t team, std::string name, const boost::property_tree::ptree& data) :
        ShipData( team, std::move(name) ),
        mSubSystems( std::make_unique<SubSystems>(data) )
{
}

Starship::~Starship() {

}

void Starship::onStep()
{
    std::vector<IComponent*> cmps;
    std::vector<double> requests;
    cmps.push_back( &getEngine() );
    cmps.push_back( &getShield() );
    cmps.push_back( mSubSystems->mPowerPlant.get() );
    for(auto& wpn : mSubSystems->mArmament)
        cmps.push_back( wpn.get() );

    float energy_to_distribute = 0;
    // get excess energy from each component
    for(auto& sys : cmps) {
        energy_to_distribute += sys->getExcessEnergy();
        requests.push_back( sys->getEnergyRequest());
    }

    mEnergyProduced = energy_to_distribute;

    // energy distribution according to demand and priority
    std::vector<double> supply( requests.size(), 0.f );

    const int num_iters = 10;
    for(unsigned p = 0; p < num_iters; ++p)
    {
        double total_request = std::accumulate( begin(requests), end(requests), 0.0 );
        if(total_request == 0)
            break;

        float edist = (p+1) / num_iters * energy_to_distribute;
        for(unsigned i = 0; i < supply.size(); ++i)
        {
            if(requests[i] == 0) continue;

            double add = requests[i] * edist / total_request * cmps[i]->energyPriority();

            if(supply[i] + add > cmps[i]->getEnergyRequest()) {
               add = cmps[i]->getEnergyRequest() - supply[i];
            }
            if(supply[i] + add > energy_to_distribute)
                add = energy_to_distribute - supply[i];

            supply[i] += add;
            requests[i] -= add;
            energy_to_distribute -= add;
        }
    }

    mEnergyUsed = std::accumulate(begin(supply), end(supply), 0.0);

    // supply energy to systems
    for(unsigned i = 0; i < supply.size(); ++i) {
        cmps[i]->provideEnergy(supply[i]);
    }

    for(auto& c : cmps)
    {
        c->onStep();
    }
}

const Engine& Starship::engine() const
{
    return *mSubSystems->mEngine;
}

Engine& Starship::getEngine()
{
    return *mSubSystems->mEngine;
}

std::size_t Starship::weapon_count() const
{
    return mSubSystems->mArmament.size();
}

const IWeapon& Starship::weapon( std::size_t id ) const
{
    return *mSubSystems->mArmament.at(id);
}

IWeapon& Starship::getWeapon( std::size_t id )
{
    return *mSubSystems->mArmament.at(id);
}

float Starship::HP() const
{
    return mHitPoints;
}

void Starship::setHP(float hp)
{
    mHitPoints = hp;
}

bool Starship::alive() const
{
    return mHitPoints > 0;
}

template<class T>
std::unique_ptr<T> clone( const std::unique_ptr<T>& original )
{
    return std::unique_ptr<T>( original->clone() );
}

template<class T>
std::vector<T> clone( const std::vector<T>& original )
{
    std::vector<T> cl;
    cl.reserve( original.size() );
    std::transform( begin(original), end(original),
                    std::back_inserter(cl), [](const T& t){ return clone(t); } );
    return std::move(cl);
}

Starship::Starship(const Starship& other):
    GameObject( other ),
    ShipData( other ),
    mSubSystems( std::make_unique<SubSystems>( *other.mSubSystems ) )
{

}

Starship::SubSystems::SubSystems( const SubSystems& other ):
        mEngine( clone(other.mEngine) ),
        mShield( clone(other.mShield) ),
        mPowerPlant( clone(other.mPowerPlant) ),
        mHull( clone(other.mHull) ),
        mArmament( clone(other.mArmament) )
{

}

Starship& Starship::operator=(const Starship& other)
{
    *this = Starship(other);
    return *this;
}

ShieldGenerator& Starship::getShield()
{
    return *mSubSystems->mShield;
}

const ShieldGenerator& Starship::shield() const
{
    return *mSubSystems->mShield;
}

Starship::Starship(Starship&& o):
    GameObject(o), ShipData(o),
    mSubSystems( std::move(o.mSubSystems) )
{

}

Starship& Starship::operator=( Starship&& o )
{
    GameObject::operator=( o );
    ShipData::operator=( o );
    mSubSystems = std::move(o.mSubSystems);
    return *this;
}

SystemStatus Starship::shield_strength() const
{
    return SystemStatus{ shield().shield(), shield().max_shield() };
}

double Starship::producedEnergy() const
{
    return mEnergyProduced;
}

double Starship::usedEnergy() const
{
    return mEnergyUsed;
}

SystemStatus Starship::hull_status() const {
    return SystemStatus{hull().armour(), hull().max_armour()};
}

const Hull& Starship::hull() const {
    return *mSubSystems->mHull;
}

Hull& Starship::getHull() {
    return *mSubSystems->mHull;
}

void Starship::dealDamage(float dmg)
{
    std::vector<IComponent*> cmps;
    cmps.push_back( &getEngine() );
    cmps.push_back( &getShield() );
    cmps.push_back( mSubSystems->mPowerPlant.get() );
    for(auto& wpn : mSubSystems->mArmament)
        cmps.push_back( wpn.get() );

    auto dmg_target = rand() % cmps.size();
    auto leftover = cmps[dmg_target]->dealDamage( dmg );
    mHitPoints -= leftover;
}

length_t spatacs::core::distance(const Starship& s1, const Starship& s2)
{
    return length(s1.position() - s2.position());
}



uint64_t ShipData::team() const
{
    return mTeam;
}

const std::string& ShipData::name() const
{
    return mName;
}

float ShipData::max_hp() const
{
    return mMaxHitPoints;
}
