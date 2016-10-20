//
// Created by erik on 10/7/16.
//

#ifndef SPATACS_SUBSYSTEMS_H
#define SPATACS_SUBSYSTEMS_H

#include <boost/property_tree/ptree_fwd.hpp>
#include "GameObject.h"
#include "core/EntityManager.h"

namespace spatacs
{
    namespace game
    {
        class EnergyManager
        {
        public:
            void process(core::EntityManager <ComponentEntity>& mgr);

            double requestPower(double amount);
        private:
            double mPowerLeft;
            double mSupplyFactor;

            double mRequested;
            double mTotal;
        };

        struct SubSystems
        {
            SubSystems(const boost::property_tree::ptree& data);
            SubSystems( const SubSystems& other );

            std::vector<ComponentEntity*> mArmament;

            double distributeEnergy(double energy);

            double produceEnergy();

            void onStep(Starship& ship);

            core::EntityManager<ComponentEntity> mComponents;
            EnergyManager mEnergyMgr;
        };
    }
}

#endif //SPATACS_SUBSYSTEMS_H
