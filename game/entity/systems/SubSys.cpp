//
// Created by erik on 1/6/17.
//

#include "SubSys.h"
#include "../entity_comps.h"

#include "game/entity/subs/managers.h"
#include "game/entity/subs/systems/Fuel.h"

using namespace spatacs;
using namespace game;
using namespace systems;


void SubCompSystem::operator()(components::SubEntities& subs)
{
    // fuel system
    {
        // try to get 10% more than was last requested, to have some buffer
        auto needed_fuel = subs.fuelmgr().requested() * 1.1 - subs.fuelmgr().reserve();
        auto got_fuel    = request_fuel(subs.submgr(), needed_fuel);
        subs.fuelmgr().provide(got_fuel);
    }

    // power system
}