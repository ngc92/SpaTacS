//
// Created by erik on 9/26/16.
//

#include <boost/throw_exception.hpp>
#include "Object.h"

using namespace spatacs;
using namespace physics;


Object::Object(const length_vec& p, const velocity_vec& v, mass_t m, std::uint64_t udata) :
        mPosition(p),
        mVelocity(v),
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

void Object::setPosition(const length_vec& p)
{
    mPosition = p;
}

void Object::setVelocity(const velocity_vec& v)
{
    mVelocity =  v;
}

std::uint64_t Object::userdata() const
{
    return mUserData;
}

void Object::setUserdata(std::uint64_t value)
{
    mUserData = value;
}

ObjectID Object::id() const
{
    return mID;
}

void Object::setID(ObjectID id)
{
    if( !id )
        BOOST_THROW_EXCEPTION( std::logic_error("New ObjectID is invalid.") );
    if( mID )
        BOOST_THROW_EXCEPTION( std::logic_error("Trying to set ID to Object has already an ID assigned.") );

    mID = id;
    for(auto& fix : mFixtures)
        fix.setParent(mID);
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

Fixture& Object::addFixture(length_t radius)
{
    mFixtures.push_back( Fixture(mID, FixtureID{0}, radius) );
    return mFixtures.back();
}

