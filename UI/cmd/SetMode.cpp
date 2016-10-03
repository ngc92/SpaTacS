//
// Created by erik on 10/1/16.
//

#include "SetMode.h"
#include <iostream>

using namespace spatacs;
using namespace cmd;

SetWpnMode::SetWpnMode(std::uint64_t object, std::uint64_t wp_id, std::uint64_t mode):
    mObject(object),
    mMode(mode),
    mWeapon(wp_id)
{

}

SetWpnMode::SetWpnMode(std::istream& in)
{
    in >> mObject >> mWeapon >> mMode;
}

std::uint64_t SetWpnMode::object() const
{
    return mObject;
}

std::uint64_t SetWpnMode::mode() const
{
    return mMode;
}

std::uint64_t SetWpnMode::weapon() const
{
    return mWeapon;
}

std::ostream& ::spatacs::cmd::operator<<(std::ostream& out, const SetWpnMode& move)
{
    return out << "SetWpnMode("<<move.object() << ", " << move.weapon() << " " << move.mode() << ")";
}
