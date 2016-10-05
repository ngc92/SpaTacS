//
// Created by erik on 9/18/16.
//

#include "Engine.h"
#include <iostream>
#include <boost/property_tree/ptree.hpp>
#include "core/Starship.h"
#include "FuelTank.h"

namespace spatacs
{
    namespace  core
    {
        void Engine::onStep(Starship& ship)
        {
            auto need_recharge = max_thrust() - mUnusedThrust;
            if(need_recharge < force_t(0))
                need_recharge = force_t(0);
            double energy_demand = mEnergyConsumption * ((need_recharge / mMaxThrust).value + 0.1);
            double fuel_demand   = mFuelConsumption * ((need_recharge / mMaxThrust).value + 0.1);
            double efac = requestEnergy(energy_demand) / energy_demand;
            double ffac = ship.getTank().getFuel( fuel_demand ) / fuel_demand * 0.1;

            mUnusedThrust += need_recharge * std::min(efac, ffac);
        }

        force_vec Engine::getThrust(const force_vec& accel)
        {
            auto want = length(accel);
            double scale = 1;
            if( want > mUnusedThrust ){
                scale = (mUnusedThrust / want).value;
                mUnusedThrust = force_t(0);
            } else {
                mUnusedThrust -= want;
            }
            return accel * scale;
        }

        Engine* Engine::clone() const
        {
            return new Engine(*this);
        }

        force_t Engine::max_thrust() const
        {
            return mMaxThrust * status();
        }

        Engine::Engine(const ptree& props):
            IComponent(props),
            mMaxThrust( props.get<float>("thrust") ),
            mEnergyConsumption( props.get<float>("energy_consumption") ),
            mFuelConsumption( props.get<float>("fuel_consumption") )
        {

        }

    }
}