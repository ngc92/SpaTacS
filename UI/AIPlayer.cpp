//
// Created by erik on 9/23/16.
//

#include "AIPlayer.h"
#include <boost/range/adaptor/indirected.hpp>
#include <events/Combat.h>
#include <iostream>

using namespace spatacs;
using namespace ui;

void AIPlayer::init()
{

}

std::vector<cmd::Command> AIPlayer::getCommands() const
{
    return mCommands;
}


void AIPlayer::setState(const core::GameState& state)
{
    mCommands.clear();
    for(auto& own : state.getShips())
    {
        if(own.team() != mOwnTeam)
            continue;

        // if the shield is damaged.
        float own_shield = own.shield_strength().current;

        length_t min = 100.0_km;
        // find closest ship to attack
        const core::Starship* target = nullptr;
        for (auto& e : state.getShips())
        {
            if (e.team() != own.team() && distance(e, own) < min) {
                min = distance(e, own);
                target = &e;
            }
        }

        if(!target)
            continue;

        // if found, do attack
        if(min < 10.0_km)  {
            mCommands.push_back( cmd::Attack(own.id(), target->id()) );
        }

        // fly closer if shield is stronger
        if(own_shield > target->shield_strength().current || own_shield > 2)
        {
            mCommands.push_back( cmd::Move(own.id(), target->position(), 0.25_kps) );
        } else
        {
            auto delta = own.position() - target->position();
            delta *= 1.0_km / length(delta);
            mCommands.push_back( cmd::Move(own.id(), own.position() + delta, 2.0_kps) );
        }
    }

    mLastState = &state;
}

void AIPlayer::notifyEvents(const std::vector<std::unique_ptr<events::IEvent>>& events)
{
    mHits.clear();
    for(const auto& ev : boost::adaptors::indirect(events))
    {
        if( dynamic_cast<const events::Hit*>(&ev) )
        {
            auto hit_id = dynamic_cast<const events::Hit&>(ev).id();
            mHits.push_back(hit_id);
        }
    }
}

bool AIPlayer::step()
{
    for(auto& hit : mHits)
    {
        if(mLastState->getShip(hit).team() == mOwnTeam)
        {
            // don't do anything for now!
        }
    }
    return false;
}

AIPlayer::AIPlayer(std::uint64_t team) : mOwnTeam( team )
{

}
