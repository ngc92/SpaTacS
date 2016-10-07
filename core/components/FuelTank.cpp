//
// Created by erik on 10/5/16.
//

#include "FuelTank.h"
#include <boost/property_tree/ptree.hpp>
#include <iostream>

using namespace spatacs;
using namespace core;

FuelTank::FuelTank(const ptree& props) :
    IComponent(props),
    mFuelCapacity( tonnes(props.get<double>("capacity")) ),
    mCurrentFuel( mFuelCapacity )
{

}

FuelTank* FuelTank::clone() const
{
    return new FuelTank(*this);
}

mass_t FuelTank::requestFuel(mass_t req)
{
    if(req < mCurrentFuel)
    {
        mCurrentFuel -= req;
        return req;
    } else
    {
        req = mCurrentFuel;
        mCurrentFuel = 0.0_kg;
        return req;
    }
}

mass_t FuelTank::fuel() const
{
    return mCurrentFuel;
}

mass_t FuelTank::capacity() const
{
    return mFuelCapacity;
}
