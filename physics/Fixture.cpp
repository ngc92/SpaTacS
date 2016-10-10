//
// Created by erik on 10/10/16.
//

#include "Fixture.h"
#include <boost/throw_exception.hpp>
#include "Object.h"

using namespace spatacs::physics;

Fixture::Fixture(std::uint64_t parent, length_t rad) :
        mRadius(rad),
        mParent(parent)
{
    if(rad < 0.0_km)
        BOOST_THROW_EXCEPTION( std::logic_error("trying to set negative radius!") );
}

length_t Fixture::radius() const
{
    return mRadius;
}

void Fixture::setRadius(length_t rad)
{
    if(rad < 0.0_km)
        BOOST_THROW_EXCEPTION( std::logic_error("trying to set negative radius!") );
    mRadius = rad;
}

std::uint64_t Fixture::userdata() const
{
    return mUserData;
}

void Fixture::setUserdata(std::uint64_t value)
{
    mUserData = value;
}

void Fixture::setParent(std::uint64_t p)
{
    mParent = p;
}

std::uint64_t Fixture::parent() const
{
    return mParent;
}
