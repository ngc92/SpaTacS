//
// Created by erik on 1/3/17.
//

#include "components.h"
#include "core/ecs/EntityManager.h"
#include <boost/algorithm/clamp.hpp>
#include <boost/throw_exception.hpp>


using namespace spatacs;
using namespace game;
using namespace components;

double Health::setCurrent(double new_health)
{
    mCurrent = boost::algorithm::clamp(new_health, 0.0, mMaximum);
    return mCurrent;
}

void Health::setMaximum(double new_maximum)
{
    if(new_maximum < 0)
        BOOST_THROW_EXCEPTION(std::logic_error("negative maximum health!"));

    mMaximum = new_maximum;
    setCurrent(mCurrent);
}

double Health::setHealthStatus(double percentage)
{
    return setCurrent(percentage * maximum());
}


PhysicsData::PhysicsData(physics::ObjectID id, mass_t mass, length_vec pos, velocity_vec vel) :
    mPhysicsID(id), mMass(mass), mPosition(pos), mVelocity(vel)
{

}

PhysicsData& PhysicsData::setPosition(const length_vec& pos)
{
    mPosition = pos;
    return *this;
}

PhysicsData& PhysicsData::setVelocity(const velocity_vec& vel)
{
    mVelocity = vel;
    return *this;
}

PhysicsData& PhysicsData::setMass(mass_t m)
{
    mMass = m;
    return *this;
}

physics::ObjectID PhysicsData::id() const
{
    return mPhysicsID;
}

PhysicsData& PhysicsData::setForce(const force_vec& force)
{
    mForce = force;
    return *this;
}

mass_t PhysicsData::mass() const
{
    return mMass;
}

const length_vec& PhysicsData::position() const
{
    return mPosition;
}

const velocity_vec& PhysicsData::velocity() const
{
    return mVelocity;
}

const force_vec& PhysicsData::force() const
{
    return mForce;
}

// ---------------------------------------------------------------------------------------------------------------------
//                                              Propulsion Control
// ---------------------------------------------------------------------------------------------------------------------

void PropulsionControl::setDesiredAcceleration(const accel_vec& desired)
{
    mDesiredAcceleration = desired;
}

const accel_vec& PropulsionControl::desired_acceleration() const
{
    return mDesiredAcceleration;
}

void PropulsionControl::setMaximumAcceleration(accel_t macc)
{
    mMaximumAcceleration = macc;
}

accel_t PropulsionControl::maximum_acceleration() const
{
    return mMaximumAcceleration;
}

// ---------------------------------------------------------------------------------------------------------------------
//                                                  Fuel Tank
// ---------------------------------------------------------------------------------------------------------------------

FuelTank::FuelTank(mass_t v) : mCapacity(v)
{
}

mass_t FuelTank::request(mass_t desire)
{
    if(desire < 0.0_kg)
        BOOST_THROW_EXCEPTION( std::logic_error("Negative mass specified!") );
    auto m = std::min(desire, mCurrent);
    mCurrent -= m;
    return m;
}

mass_t FuelTank::fill(mass_t amount)
{
    if(amount < 0.0_kg)
        BOOST_THROW_EXCEPTION( std::logic_error("Negative mass specified!") );

    auto space = mCapacity - mCurrent;
    auto m = std::min(amount, space);
    mCurrent += m;
    return amount - m;
}

mass_t FuelTank::capacity() const
{
    return mCapacity;
}