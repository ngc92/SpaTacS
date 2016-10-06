//
// Created by erik on 10/6/16.
//

#include "LifeSupport.h"
#include "core/Starship.h"
#include <boost/property_tree/ptree.hpp>
#include <iostream>

using namespace spatacs::core;

void LifeSupport::onStep(Starship& ship)
{
    auto accel = length(ship.velocity() - mLastVelocity) / 0.1_s + 9.81_m/(1.0_s)/(1.0_s);
    mLastVelocity = ship.velocity();
    double ereq = accel.value;
    /// \todo for now, nothing happens when we do not get the requested energy!
    requestEnergy( ereq );
}

LifeSupport* LifeSupport::clone() const
{
    return new LifeSupport(*this);
}

LifeSupport::LifeSupport(const ptree& dat) :
    IComponent(dat)
{

}
