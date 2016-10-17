//
// Created by erik on 9/21/16.
//

#include "ShieldGenerator.h"
#include <algorithm>
#include <iostream>
#include <boost/property_tree/ptree.hpp>

using namespace spatacs;
using game::ShieldGenerator;
using boost::property_tree::ptree;

ShieldGenerator* ShieldGenerator::clone() const
{
    return new ShieldGenerator(*this);
}

ShieldGenerator::ShieldGenerator(const ptree& props):
    IComponent(props)
{
    mEntity.add<EnergyManagement>();
    mEntity.add<Name>("shield generator");

    auto& sgd = mEntity.add<ShieldGeneratorData>();
    sgd.mShieldRecharge = scalar_t(props.get<float>("recharge")) / 1.0_s;
    sgd.mDecay =  scalar_t(std::log(1-props.get<float>("dissipation") / 100.f)) / 1.0_s;
    sgd.mEnergyPerShieldPoint = 1.f / props.get<float>("efficiency");
}

rate_t<scalar_t> ShieldGenerator::recharge_rate() const
{
    return mEntity.get<ShieldGeneratorData>().mShieldRecharge * status();
}
