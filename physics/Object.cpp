//
// Created by erik on 9/26/16.
//

#include <boost/throw_exception.hpp>
#include "Object.h"

using namespace spatacs;
using namespace physics;


Object::Object(const length_vec& p, const velocity_vec& v, length_t r, mass_t m, std::uint64_t udata) :
        mPosition(p),
        mVelocity(v),
        mRadius(r),
        mMass(m),
        mUserData(udata)
{
}

const length_vec& Object::position() const
{
    return mPosition;
}

const velocity_vec& Object::velocity() const
{
    return mVelocity;
}

length_vec Object::position(time_t dt) const
{
    return mPosition + dt*mVelocity;
}

void Object::setPosition(const length_vec& p)
{
    mPosition = p;
}

void Object::setVelocity(const velocity_vec& v)
{
    mVelocity =  v;
}

void Object::simulate(time_t dt)
{
    mPosition += dt*mVelocity;
}

length_t Object::radius() const
{
    return mRadius;
}

void Object::setRadius(length_t rad)
{
    if(rad < 0.0_km)
        BOOST_THROW_EXCEPTION( std::logic_error("trying to set negative radius!") );
    mRadius = rad;
}

std::uint64_t Object::userdata() const
{
    return mUserData;
}

void Object::setUserdata(std::uint64_t value)
{
    mUserData = value;
}

std::uint64_t Object::id() const
{
    return mID;
}

void Object::setID(std::uint64_t id)
{
    mID = id;
}

mass_t Object::mass() const
{
    return mMass;
}

void Object::setMass(mass_t mass)
{
    if(mass < 0.0_kg)
        BOOST_THROW_EXCEPTION( std::logic_error("trying to set negative mass!") );
    mMass = mass;
}
