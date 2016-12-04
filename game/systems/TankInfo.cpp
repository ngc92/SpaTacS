//
// Created by erik on 12/3/16.
//

#include "TankInfo.h"
#include "../components.h"

using namespace spatacs;
using namespace game::systems;

void TankInfo::operator()(const FuelStorage& h)
{
    mFuel     += h.current;
    mCapacity += h.capacity;
}

FuelRequest::FuelRequest(mass_t request):
    mRequest(request)
{
}
mass_t FuelRequest::fuel() const
{
    return mFuel;
}

void FuelRequest::operator()(FuelStorage& t)
{
    auto get = t.request(mRequest);
    mRequest -= get;
    mFuel += get;
}

AddFuel::AddFuel(mass_t amount) : mFuel(amount)
{

}
void AddFuel::operator()(FuelStorage& t)
{
    mFuel = t.fill(mFuel);
}

mass_t AddFuel::fuel() const
{
    return mFuel;
}