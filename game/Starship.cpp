//
// Created by erik on 9/18/16.
//

#include "GameObject.h"
#include "game/Starship.h"
#include <cmath>
#include <algorithm>
#include "game/SubSystems.h"
#include <boost/property_tree/ptree.hpp>
#include <iostream>

using namespace spatacs;
using namespace game;

ShipData::ShipData(std::uint64_t team, std::string name, const boost::property_tree::ptree& data) :
        mTeam(team),
        mName(std::move(name) ),
        mRadius( data.get<length_t>("radius") ),
        mMaxHitPoints( data.get<double>("hitpoints") ),
        mHitPoints( mMaxHitPoints ),
        mMaxArmour( data.get<double>("armour") ),
        mCurArmour( mMaxArmour ),
        mMaxShield( data.get<double>("shield") ),
        mCurShield( mMaxShield ),
        mEmptyMass( data.get<mass_t>("mass") )
{
}


Starship::Starship() : GameObject(ObjectType::STARSHIP)
{

}
Starship::Starship(std::uint64_t team, std::string name, const boost::property_tree::ptree& data) :
        GameObject(ObjectType::STARSHIP),
        ShipData(team, std::move(name), data),
        mSubSystems( std::make_unique<SubSystems>(data) )
{
}

Starship::~Starship() {

}

void Starship::onStep()
{
    mEnergyProduced = mSubSystems->produceEnergy();
    mEnergyUsed     = mSubSystems->distributeEnergy(mEnergyProduced);
    mSubSystems->onStep(*this);
}

std::size_t Starship::weapon_count() const
{
    return mSubSystems->mArmament.size();
}

const ComponentEntity& Starship::weapon( std::size_t id ) const
{
    return *mSubSystems->mArmament.at(id);
}

ComponentEntity& Starship::getWeapon( std::size_t id )
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

SystemStatus Starship::hull_status() const {
    return SystemStatus{armour(), max_armour()};
}

void Starship::dealDamage(double dmg)
{
    auto dmg_target = rand() % mSubSystems->mComponents.size();
    auto& cmp = mSubSystems->mComponents.get(dmg_target + 1);

    // one shot can only ever destroy half the component
    double d = std::min(dmg, cmp.get<Health>().current / 2);
    cmp.get<Health>().current -= d;
    mHitPoints -= dmg - d;
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

Starship* Starship::clone() const
{
    return new Starship(*this);
}

const core::EntityManager<ComponentEntity>& Starship::components() const
{
    return mSubSystems->mComponents;
}

core::EntityManager<ComponentEntity>& Starship::components()
{
    return mSubSystems->mComponents;
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

double ShipData::shield() const
{
    return mCurShield;
}

double ShipData::max_shield() const
{
    return mMaxShield;
}

void ShipData::setShield(double new_value)
{
    if(new_value < 0)
        BOOST_THROW_EXCEPTION( std::logic_error("Trying to set negative shield strength") );
    mCurShield = std::min(new_value, mMaxShield);

}

void ShipData::setDesiredAcceleration(accel_vec a)
{
    mDesiredAcceleration = a;
}

accel_vec ShipData::getProducedAcceleration()
{
    return mProducedAcceleration;
}

accel_vec ShipData::getDesiredAcceleration() const
{
    return mDesiredAcceleration;
}

void ShipData::setProducedAcceleration(accel_vec a)
{
    mProducedAcceleration = a;
}

accel_t ShipData::getMaxAcceleration() const
{
    return mMaxAccel;
}

void ShipData::setMaxAcceleration(accel_t acc)
{
    mMaxAccel = acc;
}

mass_t ShipData::getTotalMass() const
{
    return mEmptyMass + mFuelMass;
}

void ShipData::setFuelMass(mass_t mass)
{
    mFuelMass = mass;
}
