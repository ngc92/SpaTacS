//
// Created by erik on 10/5/16.
//

#include "FuelTank.h"
#include <boost/property_tree/ptree.hpp>

using namespace spatacs;
using namespace core;

FuelTank::FuelTank(const ptree& props) :
    IComponent(props),
    mFuelCapacity(props.get<double>("capacity") * 1000 /* tons */),
    mCurrentFuel( mFuelCapacity )
{

}

FuelTank* FuelTank::clone() const
{
    return new FuelTank(*this);
}

double FuelTank::getFuel(double req)
{
    if(req < mCurrentFuel)
    {
        mCurrentFuel -= req;
        return req;
    } else
    {
        req = mCurrentFuel;
        mCurrentFuel = 0;
        return req;
    }
}
