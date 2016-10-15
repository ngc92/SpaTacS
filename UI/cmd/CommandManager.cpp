//
// Created by erik on 9/22/16.
//

#include "CommandManager.h"
#include "core/GameState.h"
#include <iostream>
#include "events/Accelerate.h"
#include "core/Starship.h"
#include "core/components/IWeapon.h"
#include "events/Combat.h"

using namespace spatacs;
using cmd::CommandManager;

/// \todo currently, this is basically variant to tuple conversion.
struct UpdateVisitor : public boost::static_visitor<void>
{
    UpdateVisitor(cmd::CommandSlot& slot, std::vector<cmd::SetWpnMode>& os) : slot(slot),
                                                                              oneshots(os)
    {}

    cmd::CommandSlot& slot;
    std::vector<cmd::SetWpnMode>& oneshots;
    void operator()(const cmd::SetWpnMode& m) const
    {
        oneshots.push_back(m);
    }
    void operator()(const cmd::Move& m) const
    {
        slot.move = m;
    }
    void operator()(const cmd::Attack& m) const
    {
        slot.attack = m;
    }
};

void CommandManager::addCommand( std::uint64_t target, cmd::Command c)
{
    auto& commands = mCommandSlots[target];
    boost::apply_visitor( UpdateVisitor{commands, mOneShotCommands}, c );
}

const cmd::CommandSlot& CommandManager::getCommandsOf(std::uint64_t ship) const
{
    return mCommandSlots.at(ship);
}

void CommandManager::getCommandEvents(std::vector<events::EventPtr>& events) const
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
                std::size_t wp_count = ship.weapon_count();
                for(std::size_t i = 0; i < wp_count; ++i)
                {
                    if(ship.weapon(i).ready())
                        events.push_back( events::EventPtr(new events::FireWeapon(ship.id(), target.id(), i)) );
                }
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

        events.push_back(events::EventPtr(new events::SetWeaponMode(ship.id(), s.weapon(), s.mode())));
    }
}

void CommandManager::setState(const std::shared_ptr<const core::GameState>& state)
{
    mState = state;
    mOneShotCommands.clear();
    validate();
}


void CommandManager::validate()
{
    for(auto& p : mCommandSlots )
    {
        p.second.delflag = true;
    }

    // Iterate over all entities
    for(const core::GameObject& o : *mState)
    {
        if(o.alive() && o.type() == core::ObjectType::STARSHIP)
        {
            mCommandSlots[o.id()].delflag = false;
        }
    }

    // erase all those that are marked for deletion
    for( auto it = mCommandSlots.begin(); it != mCommandSlots.end(); ) {
        if( it->second.delflag) it = mCommandSlots.erase(it);
        else ++it;
    }
}