//
// Created by erik on 10/5/16.
//

#include "FuelTank.h"
#include <boost/property_tree/ptree.hpp>
#include <iostream>

using namespace spatacs;
using namespace game;

FuelTank::FuelTank(const ptree& props) :
    IComponent(props)
{
    mEntity.add<FuelStorage>( props.get<mass_t>("capacity") );
}

FuelTank* FuelTank::clone() const
{
    return new FuelTank(*this);
}

mass_t FuelTank::requestFuel(mass_t req)
{
    if(req < mEntity.get<FuelStorage>().current)
    {
        mEntity.get<FuelStorage>().current -= req;
        return req;
    } else
    {
        req = mEntity.get<FuelStorage>().current;
        mEntity.get<FuelStorage>().current = 0.0_kg;
        return req;
    }
}

mass_t FuelTank::fuel() const
{
    return mEntity.get<FuelStorage>().current;
}

mass_t FuelTank::capacity() const
{
    return mEntity.get<FuelStorage>().capacity;
}
