//
// Created by erik on 9/30/16.
//

#include "PowerPlant.h"
#include <boost/property_tree/ptree.hpp>

using namespace spatacs;

void core::PowerPlant::onStep(Starship& ship)
{
    provideEnergy( 0.1f * power() );
}

core::PowerPlant* core::PowerPlant::clone() const
{
    return new PowerPlant(*this);
}

core::PowerPlant::PowerPlant(const ptree& data) :
    IComponent( data ),
    mEnergyProduction( data.get<float>("power") )
{

}

float core::PowerPlant::power() const
{
    return mEnergyProduction * status();
}
