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

void CommandManager::addCommand(cmd::Command c)
{
    auto ship = get_ship(c);
    auto& commands = mCommandSlots[ship];
    boost::apply_visitor( UpdateVisitor{commands, mOneShotCommands}, c );
}

void CommandManager::validate(const core::GameState& state)
{
    mOneShotCommands.clear();
    // remove commands of dead ships
    for(auto it = begin(mCommandSlots); it != end(mCommandSlots); )
    {
        try {
            auto& sp = state.getShip(it->first);
            if(!sp.alive()) {
                it = mCommandSlots.erase(it);
                continue;
            }
            if(it->second.attack)
            {
                auto& sp2 = state.getShip(it->second.attack.get().target());
                if(!sp2.alive()) {
                    it = mCommandSlots.erase(it);
                    continue;
                }
            }
            ++it;
        } catch (...) {
            it = mCommandSlots.erase( it );
        }
    }
}

void CommandManager::transcribe(const core::GameState& state, std::vector<events::EventPtr>& events) const
{
    for(auto& c : mCommandSlots)
    {
        if(c.second.attack)
        {
            auto& command = c.second.attack.get();
            auto& ship   = state.getShip(command.object());
            auto& target = state.getShip(command.target());
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
        if(c.second.move)
        {
            auto& command = c.second.move.get();
            auto& ship = state.getShip(command.object());
            if(ship.alive()) {
                // and handle the command
                auto dv = command.calcThrust(ship);
                events.push_back(events::EventPtr(new events::Accelerate(ship.id(), dv)));
            }
        }
    }

    for(auto& s : mOneShotCommands) {
        auto& ship = state.getShip(s.object());

        // dead ships don't shoot
        if (!ship.alive()) {
            continue;
        }

        events.push_back(events::EventPtr(new events::SetWeaponMode(ship.id(), s.weapon(), s.mode())));
    }
}
