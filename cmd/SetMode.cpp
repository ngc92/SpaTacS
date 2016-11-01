//
// Created by erik on 10/1/16.
//

#include "SetMode.h"
#include <iostream>

using namespace spatacs;
using namespace cmd;

SetWpnMode::SetWpnMode(std::uint64_t object, std::uint64_t wp_id, std::string ammo):
    mObject(object),
    mAmmo(ammo),
    mWeapon(wp_id)
{

}

std::uint64_t SetWpnMode::object() const
{
    return mObject;
}

const std::string& SetWpnMode::ammo() const
{
    return mAmmo;
}

std::uint64_t SetWpnMode::weapon() const
{
    return mWeapon;
}

std::ostream& ::spatacs::cmd::operator<<(std::ostream& out, const SetWpnMode& move)
{
    return out << "SetWpnMode("<<move.object() << ", " << move.weapon() << " " << move.ammo() << ")";
}