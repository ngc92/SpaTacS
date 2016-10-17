//
// Created by erik on 9/18/16.
//

#include "Engine.h"
#include <iostream>
#include <boost/property_tree/ptree.hpp>
#include "game/Starship.h"
#include "FuelTank.h"

using namespace spatacs;
using namespace game;

Engine* Engine::clone() const
{
    return new Engine(*this);
}

Engine::Engine(const ptree& props):
    IComponent(props)
{
    mEntity.add<Name>("engine");
    mEntity.add<EngineData>(props.get<speed_t>("propellant_speed"),
                            props.get<physics::rate_t<mass_t>>("fuel_consumption") );
}

