//
// Created by erik on 9/30/16.
//

#include "PowerPlant.h"
#include <boost/property_tree/ptree.hpp>

using namespace spatacs;
using namespace game;

PowerPlant* game::PowerPlant::clone() const
{
    return new PowerPlant(*this);
}

PowerPlant::PowerPlant(const ptree& data) :
    IComponent( data )
{
    mEntity.add<EnergyManagement>();
    mEntity.add<PowerPlantData>( data.get<double>("power") );
}

