//
// Created by erik on 12/3/16.
//

#include "Ammunition.h"

using namespace spatacs;
using namespace game::systems;


void ListAmmunition::operator()(const AmmoStorage& as)
{
    mCapacity += as.capacity;
    for(auto& ammo : as.ammo)
    {
        mAmmos.push_back(ammo);
    }
}

AddAmmunition::AddAmmunition(const AmmoData& a, std::size_t amount) :
        mData(a), mAmount(amount)
{

};

void AddAmmunition::operator()(AmmoStorage& as)
{
    as.addAmmo(mData, mAmount);
}

GetAmmunition::GetAmmunition(std::string type) :
        mType( std::move(type) ), mData(nullptr)
{
};

void GetAmmunition::operator()(AmmoStorage& as)
{
    if(has())
        return;

    auto& ammo = as.getAmmo(mType);
    if(ammo.amount > 0)
    {
        ammo.amount -= 1;
        mData = &ammo.data;
    }
}

bool GetAmmunition::has() const
{
    return mData != nullptr;
}

const game::AmmoData& GetAmmunition::ammo_data() const
{
    return *mData;
}