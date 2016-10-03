//
// Created by erik on 9/22/16.
//

#include "CommandManager.h"
#include "core/GameState.h"
#include <iostream>

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
            state.getShip(it->first);
            if(it->second.attack)
            {
                state.getShip(it->second.attack.get().target());
            }
            ++it;
        } catch (...) {
            it = mCommandSlots.erase( it );
        }
    }
}

std::vector<cmd::Command> CommandManager::getCommands() const
{
    auto vec = std::vector<cmd::Command>();
    vec.reserve( mCommandSlots.size() * 2 );
    for(auto& c : mCommandSlots)
    {
        if(c.second.attack)
            vec.push_back(c.second.attack.get());
        if(c.second.move)
            vec.push_back(c.second.move.get());
    }
    std::copy( begin(mOneShotCommands), end(mOneShotCommands), std::back_inserter(vec) );

    return std::move(vec);
}
