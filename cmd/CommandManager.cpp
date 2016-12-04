//
// Created by erik on 9/22/16.
//

#include "CommandManager.h"
#include "game/GameState.h"
#include <iostream>
#include "events/Accelerate.h"
#include "game/Starship.h"
#include "events/Combat.h"
#include "core/ecs/EntityManager.h"
#include "game/systems/fwd.h"
#include <boost/range/algorithm/copy.hpp>

using namespace spatacs;
using cmd::CommandManager;

const cmd::CommandSlot& CommandManager::getCommandsOf(game::ObjectID ship) const
{
    return mCommandSlots.at(ship);
}

void CommandManager::getCommandEvents(std::vector<events::EventPtr>& events)
{
    for(auto& c : mCommandSlots)
    {
        if(c.second.attack)
        {
            auto& command = c.second.attack.get();
            auto& ship   = mState->getShip(c.first);
            auto& target = mState->getObject(command.target());
            /// \todo prevent deliberate friendly fire

            // dead ships don't shoot
            if(ship.alive() && target.alive())
            {
                auto make_event = [&](auto id)
                {
                    return events::EventPtr(new events::FireWeapon(ship.id(), target.id(), id));
                };

                auto wpn_id_range = ship.components().get_matching_ids(core::type_v<game::systems::signatures::AimSignature>);
                boost::copy(wpn_id_range | boost::adaptors::transformed( make_event ), std::back_inserter(events));
            }
        }

        auto& command = c.second.move;
        auto& ship = mState->getShip(c.first);
        if(ship.alive()) {
            // and handle the command
            command.update(ship, *mState);
            auto dv = command.thrust(ship);
            events.push_back(events::EventPtr(new events::Accelerate(ship.id(), dv)));
        }

    }

    for(auto& s : mOneShotCommands) {
        /// \todo this is ugly, really ugly
        if(s.type() == typeid(SetWpnMode)) {
            auto& wm = boost::get<SetWpnMode>(s);
            auto& ship = mState->getObject(wm.object());
            // dead ships don't shoot
            if (!ship.alive()) {
                continue;
            }

            events.push_back(events::EventPtr(new events::SetWeaponAmmo(ship.id(), wm.weapon(), wm.ammo())));
        }else if(s.type() == typeid(SetSystemActivity)) {
            auto& sa = boost::get<SetSystemActivity>(s);
            auto& ship = mState->getObject(sa.object());
            // dead ships don't shoot
            if (!ship.alive()) {
                continue;
            }

            events.push_back(events::EventPtr(new events::SetSystemActivity(ship.id(), sa.system(), sa.activity())));
        }
    }

    mOneShotCommands.clear();
}

void CommandManager::setState(const state_t& state)
{
    mState = std::dynamic_pointer_cast<const game::GameState>(state);
    validate();
}


void CommandManager::validate()
{
    for(auto& p : mCommandSlots )
    {
        p.second.delflag = true;
    }

    // Iterate over all entities
    for(const game::GameObject& o : *mState)
    {
        if(o.alive() && o.type() == game::ObjectType::STARSHIP)
        {
            mCommandSlots[o.id()].delflag = false;
        }
    }

    // erase all those that are marked for deletion
    for( auto it = mCommandSlots.begin(); it != mCommandSlots.end(); ) {
        if( it->second.delflag) it = mCommandSlots.erase(it);
        else ++it;
    }

    // remove invalid attacks
    for(auto& cmd : mCommandSlots)
    {
        if(cmd.second.attack)
        {
            if(!mState->getObject(cmd.second.attack.get().target()).alive())
                cmd.second.attack = boost::none;
        }
    }
}

void CommandManager::addCommand(game::ObjectID target, cmd::Move cmd)
{
    mCommandSlots[target].move = std::move(cmd);
}

void CommandManager::addCommand(game::ObjectID target, cmd::Attack cmd)
{
    mCommandSlots[target].attack = std::move(cmd);
}

void CommandManager::addCommand(game::ObjectID target, cmd::SetWpnMode cmd)
{
    mOneShotCommands.push_back(std::move(cmd));
}

void CommandManager::addCommand(game::ObjectID target, cmd::SetSystemActivity cmd)
{
    mOneShotCommands.push_back(std::move(cmd));
}
