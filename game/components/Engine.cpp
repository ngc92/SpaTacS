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

void Engine::onStep(Starship& ship)
{
    force_t want    = length(mDesiredAcceleration);
    // if we do not want to accelerate, end here
    if(want == force_t(0))
        return;

    mass_t need_mass  = (want / mPropellantSpeed) * 0.1_s;
    mass_t max_mass   = mMassRate * 0.1_s * status();
    if(need_mass > max_mass)
        need_mass = max_mass;
    mass_t fuel = ship.getTank().requestFuel( need_mass );

    force_t f = fuel * mPropellantSpeed  / 0.1_s;
    force_vec force = mDesiredAcceleration * (f / want);
}

Engine* Engine::clone() const
{
    return new Engine(*this);
}

force_t Engine::max_thrust() const
{
    return mPropellantSpeed * mMassRate * status();
}

Engine::Engine(const ptree& props):
    IComponent(props),
    mPropellantSpeed( kilometers(props.get<float>("propellant_speed")) / 1.0_s ),
    mMassRate( kilogram(props.get<float>("fuel_consumption")) / 1.0_s )
{

}

void Engine::setTargetAccel(accel_vec a)
{
    mDesiredAcceleration = a;
}
