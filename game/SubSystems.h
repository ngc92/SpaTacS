//
// Created by erik on 10/7/16.
//

#ifndef SPATACS_SUBSYSTEMS_H
#define SPATACS_SUBSYSTEMS_H

#include <boost/property_tree/ptree_fwd.hpp>
#include "GameObject.h"
#include "components.h"
#include "core/ecs/EntityManager.h"

namespace spatacs
{
    namespace game
    {

        class EnergyManager
        {
        public:
            void process(SubsystemManager& mgr);

            energy_t requestPower(energy_t amount);

            energy_t total() const { return mTotal; }
            energy_t requested() const { return mRequested; }
        private:
            energy_t mPowerLeft;
            double mSupplyFactor;

            energy_t mRequested;
            energy_t mTotal;
        };

        struct SubSystems
        {
            SubSystems(const boost::property_tree::ptree& data);

            void onStep(Starship& ship);
            double dealDamage(double dmg);

            SubsystemManager mComponents;
            EnergyManager mEnergyMgr;
        };
    }
}

#endif //SPATACS_SUBSYSTEMS_H
