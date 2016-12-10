//
// Created by erik on 12/10/16.
//

#include "Shield.h"

using namespace spatacs;
using namespace game;

bool Shield::alive() const
{
    return true;
}

void Shield::onStep()
{
    double decay = std::exp( mShieldDecay*0.1_s );
    mCurShield *= decay;
}

Shield* Shield::clone() const
{
    return new Shield(*this);
}

Shield::Shield(ObjectID ship) : GameObject(ObjectType::SHIELD)
{

}

double Shield::shield() const
{
    return mCurShield;
}

double Shield::max_shield() const
{
    return mMaxShield;
}

void Shield::setShield(double new_value)
{
    if(new_value < 0)
        new_value = 0;
    mCurShield = std::min(new_value, mMaxShield);
}

length_t Shield::radius() const
{
    return mRadius;
}

void Shield::setRadius(length_t radius)
{
    mRadius = radius;
}
