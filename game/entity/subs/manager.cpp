//
// Created by erik on 1/6/17.
//

#include "managers.h"

using namespace spatacs;
using namespace game;

mass_t FuelDistributor::request(mass_t amount)
{
    if(amount < 0.0_kg)
        amount = 0.0_kg;

    mTotalRequests += amount;
    amount = std::min(amount, mCache);
    mCache -= amount;
    return amount;
}

mass_t FuelDistributor::requested() const
{
    return mTotalRequests;
}

void FuelDistributor::provide(mass_t f)
{
    mCache += f;
    mTotalRequests = 0.0_kg;
}

mass_t FuelDistributor::reserve() const
{
    return mCache;
}






energy_t PowerDistributor::supply(energy_t provide)
{
    mTotalRequested = 0.0_J;
    mCache = provide;
    mTotalProduced = provide;
}

energy_t PowerDistributor::request(energy_t amount)
{
    if(amount < 0.0_J)
        amount = 0.0_J;

    mTotalRequested += amount;
    amount = std::min(amount, mCache);
    mCache -= amount;
    return amount;
}

energy_t PowerDistributor::produced() const
{
    return mTotalProduced;
}

energy_t PowerDistributor::requested() const
{
    return mTotalRequested;
}
