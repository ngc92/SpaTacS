//
// Created by erik on 9/21/16.
//

#ifndef SOI_ICOMPONENT_H
#define SOI_ICOMPONENT_H

#include <memory>
#include <boost/property_tree/ptree_fwd.hpp>
#include "comps.h"

namespace spatacs
{
    namespace game
    {
        // creation functions
        void makeEngine(const boost::property_tree::ptree& data, ComponentEntity& cmp);
        void makeFuelTank(const boost::property_tree::ptree& data, ComponentEntity& cmp);
        void makeLifeSupport(const boost::property_tree::ptree& data, ComponentEntity& cmp);
        void makePowerPlant(const boost::property_tree::ptree& data, ComponentEntity& cmp);
        void makeProjectileWpn(const boost::property_tree::ptree& data, ComponentEntity& cmp);
        void makeShieldGenerator(const boost::property_tree::ptree& data, ComponentEntity& cmp);
    }
}

#endif //SOI_ICOMPONENT_H
