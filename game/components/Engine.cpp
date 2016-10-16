//
// Created by erik on 9/18/16.
//

#include "Engine.h"
#include <iostream>
#include <boost/property_tree/ptree.hpp>
#include "game/Starship.h"
#include "FuelTank.h"

namespace spatacs
{
    namespace  game
    {
        void Engine::onStep(Starship& ship)
        {
            auto mass_diff = mMassRate * 0.1_s;
            mass_t fuel_demand = mass_diff - mUnusedMass;
            if(fuel_demand < 0.0_kg)
                fuel_demand = 0.0_kg;

            mUnusedMass += ship.getTank().requestFuel(fuel_demand * status());
        }

        force_vec Engine::getThrust(const force_vec& accel)
        {
            force_t want    = length(accel);
            if(want == force_t(0))
                return accel;

            mass_t need_mass  = (want / mPropellantSpeed) * 0.1_s;
            if( need_mass > mUnusedMass ) {
                need_mass = mUnusedMass;
            }

            force_t f = need_mass * mPropellantSpeed / 0.1_s;
            mUnusedMass -= need_mass;
            return accel * (f / want);
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

    }
}