//
// Created by erik on 10/1/16.
//

#include "SetMode.h"
#include <iostream>
#include <game/ObjectID.h>

using namespace spatacs;
using namespace cmd;

SetWpnMode::SetWpnMode(game::ObjectID object, game::CompEntID wp_id, std::string ammo):
    mObject(object),
    mAmmo(ammo),
    mWeapon(wp_id)
{

}

game::ObjectID SetWpnMode::object() const
{
    return mObject;
}

const std::string& SetWpnMode::ammo() const
{
    return mAmmo;
}

game::CompEntID SetWpnMode::weapon() const
{
    return mWeapon;
}

std::ostream& ::spatacs::cmd::operator<<(std::ostream& out, const SetWpnMode& move)
{
    return out << "SetWpnMode("<<move.object() << ", " << move.weapon() << " " << move.ammo() << ")";
}


SetSystemActivity::SetSystemActivity(game::ObjectID object, game::CompEntID system_id, double activity) :
    mObject(object), mSystem(system_id), mActivity(activity)
{

}

game::ObjectID SetSystemActivity::object() const
{
    return mObject;
}
double SetSystemActivity::activity() const
{
    return mActivity;
}

game::CompEntID SetSystemActivity::system() const
{
    return mSystem;
}