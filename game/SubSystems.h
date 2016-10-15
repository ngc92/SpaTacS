//
// Created by erik on 10/7/16.
//

#ifndef SPATACS_SUBSYSTEMS_H
#define SPATACS_SUBSYSTEMS_H

#include <boost/property_tree/ptree_fwd.hpp>

// include all that stuff in the header, so defaulted
// constructors work.
#include "game/components/IWeapon.h"
#include "game/components/ShieldGenerator.h"
#include "game/components/Engine.h"
#include "game/components/FuelTank.h"
#include "game/components/LifeSupport.h"
#include "game/components/PowerPlant.h"
#include "GameObject.h"

namespace spatacs
{
    namespace game
    {
        struct SubSystems
        {
            SubSystems(const boost::property_tree::ptree& data);
            SubSystems( const SubSystems& other );

            std::unique_ptr<Engine> mEngine;
            std::unique_ptr<ShieldGenerator> mShield;
            std::unique_ptr<PowerPlant> mPowerPlant;
            std::unique_ptr<FuelTank> mFuelTank;
            std::unique_ptr<LifeSupport> mLifeSupport;
            std::vector<std::unique_ptr<IWeapon>> mArmament;

            std::vector<IComponent*> mCompPtrs;

            double distributeEnergy(double energy);

            double produceEnergy() const;
        };
    }
}

#endif //SPATACS_SUBSYSTEMS_H
