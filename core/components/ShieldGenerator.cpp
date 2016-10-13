//
// Created by erik on 9/21/16.
//

#include "ShieldGenerator.h"
#include <algorithm>
#include <iostream>
#include <boost/property_tree/ptree.hpp>

using namespace spatacs;
using core::ShieldGenerator;
using boost::property_tree::ptree;

float ShieldGenerator::shield() const
{
    return mCurrentShieldStrength;
}

float ShieldGenerator::max_shield() const
{
    return mMaximumShieldStrength;
}

void ShieldGenerator::onStep(Starship& ship)
{
    auto dt = 0.1_s;
    // shield decay
    double decay = std::exp( mDecay*dt );
    mCurrentShieldStrength *= decay;
    if(shield() < max_shield())
    {
        double difference = max_shield() - shield();
        double rec = std::min( recharge_rate() * .1, difference );
        double egy = rec * mEnergyPerShieldPoint;
        rec *= requestEnergy( egy ) / egy;
        mCurrentShieldStrength += rec;
        update_cooldown( rec );
    }
}

ShieldGenerator& ShieldGenerator::setShield(float s)
{
    if(s < 0)
        BOOST_THROW_EXCEPTION( std::logic_error("Trying to set negative shield strength") );
    mCurrentShieldStrength = std::min(s, mMaximumShieldStrength);
    return *this;
}

ShieldGenerator* ShieldGenerator::clone() const
{
    return new ShieldGenerator(*this);
}

ShieldGenerator::ShieldGenerator(const ptree& props):
    IComponent(props),
    mMaximumShieldStrength( props.get<float>("strength") ),
    mShieldRecharge( props.get<float>("recharge") ),
    mDecay( std::log(1-props.get<float>("dissipation") / 100.f) ),
    mEnergyPerShieldPoint( 1.f / props.get<float>("efficiency") )
{
    mCurrentShieldStrength = mMaximumShieldStrength;
}

double ShieldGenerator::recharge_rate() const
{
    return mShieldRecharge * status() / temperature();
}
