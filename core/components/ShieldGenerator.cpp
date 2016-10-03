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

void ShieldGenerator::onStep()
{
    // shield decay
    float decay = std::exp( std::log(1-mDecayPerSecond)*0.1f);
    mCurrentShieldStrength *= decay;
    if(shield() < max_shield())
    {
        float difference = max_shield() - shield();
        float rec = std::min( recharge_rate() * .1f, difference );
        float egy = rec * mEnergyPerShieldPoint;
        rec *= requestEnergy( egy ) / egy;
        mCurrentShieldStrength += rec;
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

float ShieldGenerator::getDeflectionChance(const Vec& displacement, const Vec& impact_impulse) const
{
    Vec normal = parallel(impact_impulse, displacement);
    float nimp = length(normal);
    float rdsp = nimp / mCurrentShieldStrength;
    return std::tanh( .1f / rdsp );
}

ShieldGenerator::ShieldGenerator(const ptree& props):
    IComponent(props),
    mMaximumShieldStrength( props.get<float>("strength") ),
    mShieldRecharge( props.get<float>("recharge") ),
    mDecayPerSecond( props.get<float>("dissipation") / 100.f ),
    mEnergyPerShieldPoint( 1.f / props.get<float>("efficiency") )
{
    mCurrentShieldStrength = mMaximumShieldStrength;
}

float ShieldGenerator::recharge_rate() const
{
    return mShieldRecharge * status();
}
