//
// Created by erik on 9/18/16.
//

#include "GameObject.h"
#include "GameState.h"
#include "game/Starship.h"
#include <cmath>
#include <algorithm>
#include "game/SubSystems.h"
#include "Shield.h"
#include <boost/property_tree/ptree.hpp>
#include <iostream>

using namespace spatacs;
using namespace game;

ShipData::ShipData(std::uint64_t team, std::string name, const boost::property_tree::ptree& data) :
        mMaxHitPoints( data.get<double>("hitpoints") ),
        mHitPoints( mMaxHitPoints ),
        /*,
        mMaxShield( data.get<double>("shield") ),
        mCurShield( mMaxShield ),
        mShieldDecay( scalar_t(std::log(1-data.get<float>("shield_dissipation") / 100.f)) / 1.0_s ),
         */
        mMaxArmour( data.get<double>("armour") ),
        mCurArmour( mMaxArmour ),
        mEmptyMass( data.get<mass_t>("mass") ),
        mRadius( data.get<length_t>("radius") ),
        mTeam(team),
        mName(std::move(name) )
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

void Starship::onStep(GameState& state)
{
    mSubSystems->onStep(*this, state);
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

void Starship::dealDamage(double dmg)
{
    dmg = mSubSystems->dealDamage(dmg);
    mHitPoints -= dmg;
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

const SubsystemManager& Starship::components() const
{
    return mSubSystems->mComponents;
}

SubsystemManager& Starship::components()
{
    return mSubSystems->mComponents;
}

energy_t Starship::getProducedEnergy() const
{
    return mSubSystems->mEnergyMgr.total();
}

energy_t Starship::getUsedEnergy() const
{
    return mSubSystems->mEnergyMgr.requested();
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

double ShipData::shield_strength(const GameState& state) const
{
    try {
        auto& assoc_shield = dynamic_cast<const Shield&>(state.getObject(mShieldObjectID));
        return assoc_shield.current();
    } catch (...) {
        /// \todo this is very ugly!!
        return 0;
    }
}

ObjectID ShipData::shield_id() const
{
    return mShieldObjectID;
}

void ShipData::setShieldID(ObjectID shield)
{
    mShieldObjectID = shield;
}
