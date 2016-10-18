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
        struct SubSystems
        {
            SubSystems(const boost::property_tree::ptree& data);
            SubSystems( const SubSystems& other );

            std::vector<ComponentEntity*> mArmament;

            double distributeEnergy(double energy);

            double produceEnergy();

            void onStep(Starship& ship);

            core::EntityManager<ComponentEntity> mComponents;
        };
    }
}

#endif //SPATACS_SUBSYSTEMS_H
