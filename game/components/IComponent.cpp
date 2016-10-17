//
// Created by erik on 9/21/16.
//

#include "IComponent.h"
#include <boost/property_tree/ptree.hpp>

namespace spatacs
{
    namespace game
    {

        double IComponent::hp() const
        {
            return mEntity.get<Health>().current;
        }

        double IComponent::max_hp() const
        {
            return mEntity.get<Health>().maximum;
        }

        IComponent::IComponent(const IComponent::ptree& data) :
            mEntity(0)
        {
            mEntity.add<Health>( data.get<float>("HP") );
        }

        double IComponent::status() const
        {
            return hp() / max_hp();
        }

        double IComponent::dealDamage(double dmg)
        {
            // one shot can only ever destroy half the component
            double d = std::min(dmg, hp() / 2);
            mEntity.get<Health>().current -= d;
            return dmg - d;
        }
    }
}
