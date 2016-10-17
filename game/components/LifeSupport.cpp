//
// Created by erik on 10/6/16.
//

#include "LifeSupport.h"
#include "game/Starship.h"
#include <boost/property_tree/ptree.hpp>
#include <iostream>

using namespace spatacs::game;

LifeSupport* LifeSupport::clone() const
{
    return new LifeSupport(*this);
}

LifeSupport::LifeSupport(const ptree& dat) :
    IComponent(dat)
{
    mEntity.add<EnergyManagement>();
    mEntity.add<LifeSupportData>();
}
