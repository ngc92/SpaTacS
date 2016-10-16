//
// Created by erik on 9/30/16.
//

#include "PowerPlant.h"
#include <boost/property_tree/ptree.hpp>

using namespace spatacs;

void game::PowerPlant::onStep(Starship& ship)
{
    provideEnergy( 0.1f * power() );
}

game::PowerPlant* game::PowerPlant::clone() const
{
    return new PowerPlant(*this);
}

game::PowerPlant::PowerPlant(const ptree& data) :
    IComponent( data ),
    mEnergyProduction( data.get<float>("power") )
{

}

float game::PowerPlant::power() const
{
    return mEnergyProduction * status();
}
