//
// Created by erik on 9/21/16.
//

#include "IComponent.h"
#include <boost/property_tree/ptree.hpp>

namespace spatacs
{
    namespace game
    {
        double IComponent::requestEnergy(double amount)
        {
            mLastTotalRequest += amount;
            if(amount < mEnergyCache) {
                mEnergyCache -= amount;
                return amount;
            }
            float ret = mEnergyCache;
            mEnergyCache = 0;
            return ret;
        }

        void IComponent::produceEnergy(double amount)
        {
            mEnergyCache += amount;
        }

        double IComponent::getExcessEnergy() const
        {
            return mEnergyCache;
        }

        double IComponent::getEnergyRequest() const
        {
            return mLastTotalRequest;
        }

        void IComponent::provideEnergy(double offer)
        {
            mEnergyCache = offer;
            mLastTotalRequest = 0;
        }

        float IComponent::energyPriority() const
        {
            return mEnergyPriority;
        }

        void IComponent::setEnergyPriority(float p)
        {
            if(p <= 1)
                mEnergyPriority = 1;
            else
                mEnergyPriority = p;
        }

        float IComponent::hp() const
        {
            return mHitPoints;
        }

        float IComponent::max_hp() const
        {
            return mMaxHitPoints;
        }

        IComponent::IComponent(const IComponent::ptree& data) :
            mMaxHitPoints( data.get<float>("HP") ),
            mHitPoints( mMaxHitPoints )
        {

        }

        double IComponent::status() const
        {
            return hp() / max_hp();
        }

        float IComponent::dealDamage(float dmg)
        {
            // one shot can only ever destroy half the component
            float d = std::min(dmg, mHitPoints / 2);
            mHitPoints -= d;
            return dmg - d;
        }

        double IComponent::temperature() const
        {
            return 1 + mTemperatur;
        }

        void IComponent::update_cooldown(double ein)
        {
            mTemperatur += ein;
            mTemperatur *= std::exp(-0.5*0.1);
        }

    }
}
