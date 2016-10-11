//
// Created by erik on 9/18/16.
//

#include "GameObject.h"
#include "Starship.h"
#include <cmath>
#include <algorithm>
#include "components/ProjectileWeapon.h"
#include "components/ShieldGenerator.h"
#include "components/Engine.h"
#include "components/FuelTank.h"
#include "components/LifeSupport.h"
#include "SubSystems.h"
#include <boost/property_tree/ptree.hpp>
#include <iostream>
#include "components/PowerPlant.h"

using namespace spatacs;
using namespace core;

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
    mEnergyProduced = mSubSystems->produceEnergy();
    mEnergyUsed     = mSubSystems->distributeEnergy(mEnergyProduced);

    for(auto& c : mSubSystems->mCompPtrs)
    {
        c->onStep(*this);
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

bool Starship::alive() const
{
    return mHitPoints > 0;
}

Starship::Starship(const Starship& other):
    GameObject( other ),
    ShipData( other ),
    mSubSystems( std::make_unique<SubSystems>( *other.mSubSystems ) )
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

SystemStatus Starship::hull_status() const {
    return SystemStatus{armour(), max_armour()};
}

void Starship::dealDamage(float dmg)
{
    auto dmg_target = rand() % mSubSystems->mCompPtrs.size();
    auto leftover = mSubSystems->mCompPtrs[dmg_target]->dealDamage( dmg );
    mHitPoints -= leftover;
}

uint64_t ShipData::team() const
{
    return mTeam;
}

const std::string& ShipData::name() const
{
    return mName;
}

double ShipData::max_hp() const
{
    return mMaxHitPoints;
}

double ShipData::producedEnergy() const
{
    return mEnergyProduced;
}

double ShipData::usedEnergy() const
{
    return mEnergyUsed;
}

double ShipData::hp() const
{
    return mHitPoints;
}

double ShipData::armour() const
{
    return mCurArmour;
}

double ShipData::max_armour() const
{
    return mMaxArmour;
}

void ShipData::setArmour(double new_value)
{
    mCurArmour = new_value;
}

const FuelTank& Starship::tank() const
{
    return *mSubSystems->mFuelTank;
}

FuelTank& Starship::getTank()
{
    return *mSubSystems->mFuelTank;
}

Starship* Starship::clone() const
{
    return new Starship(*this);
}

void ShipData::setHP(double hp)
{
    mHitPoints = hp;
}

length_t ShipData::radius() const
{
    return mRadius;
}

void ShipData::setRadius(length_t radius)
{
    mRadius = radius;
}
