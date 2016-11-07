//
// Created by erik on 9/22/16.
//

#include "CommandManager.h"
#include "core/GameState.h"
#include <iostream>
#include <game/systems.h>
#include "events/Accelerate.h"
#include "game/Starship.h"
#include "events/Combat.h"

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
            auto& target = mState->getShip(command.target());
            /// \todo prevent deliberate friendly fire

            // dead ships don't shoot
            if(ship.alive() && target.alive())
            {

                auto fire_event_generator = game::for_each_weapon_id([&](std::size_t id){
                    events.push_back( events::EventPtr(new events::FireWeapon(ship.id(), target.id(), id)));
                });
                ship.components().apply(fire_event_generator);
            }
        }

        auto& command = c.second.move;
        auto& ship = mState->getShip(c.first);
        if(ship.alive()) {
            // and handle the command
            auto dv = command.calcThrust(ship);
            events.push_back(events::EventPtr(new events::Accelerate(ship.id(), dv)));
        }

    }

    for(auto& s : mOneShotCommands) {
        auto& ship = mState->getShip(s.object());
        // dead ships don't shoot
        if (!ship.alive()) {
            continue;
        }

        events.push_back(events::EventPtr(new events::SetWeaponAmmo(ship.id(), s.weapon(), s.ammo())));
    }

    mOneShotCommands.clear();
}

void CommandManager::setState(const std::shared_ptr<const core::GameState>& state)
{
    mState = state;
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
