//
// Created by erik on 9/20/16.
//

#include "Hull.h"
#include <boost/property_tree/ptree.hpp>

namespace spatacs
{
    namespace core
    {

        Hull::Hull(const ptree& data) :
            IComponent( data ),
            mMaxArmour( data.get<float>("armour") ),
            mCurArmour( mMaxArmour )
        {

        }

        void Hull::onStep()
        {

        }

        float Hull::max_armour() const
        {
            return mMaxArmour;
        }

        float Hull::armour() const
        {
            return mCurArmour;
        }

        Hull* Hull::clone() const
        {
            return new Hull(*this);
        }

        void Hull::setArmour(float new_value) {
            if(new_value < 0)
                BOOST_THROW_EXCEPTION(std::logic_error("Setting armour to negative value!"));
            if(new_value > mMaxArmour)
                BOOST_THROW_EXCEPTION(std::logic_error("Setting armour to value > max!"));
            mCurArmour = new_value;
        }
    }
}