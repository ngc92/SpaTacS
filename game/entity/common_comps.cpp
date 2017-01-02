//
// Created by erik on 1/2/17.
//

#include "common_comps.h"
#include <boost/algorithm/clamp.hpp>
#include <boost/throw_exception.hpp>

using namespace spatacs;
using namespace game;
using namespace components;

double Health::setCurrent(double new_health)
{
    mCurrent = boost::algorithm::clamp(new_health, 0.0, mMaximum);
    return mCurrent;
}

void Health::setMaximum(double new_maximum)
{
    if(new_maximum < 0)
        BOOST_THROW_EXCEPTION(std::logic_error("negative maximum health!"));

    mMaximum = new_maximum;
    setCurrent(mCurrent);
}

double Health::setHealthStatus(double percentage)
{
    return setCurrent(percentage * maximum());
}
