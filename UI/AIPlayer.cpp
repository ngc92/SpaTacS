//
// Created by erik on 9/23/16.
//

#include "AIPlayer.h"
#include <boost/range/adaptor/indirected.hpp>
#include <events/Combat.h>
#include <iostream>
#include "core/Starship.h"
#include "core/GameState.h"

using namespace spatacs;
using namespace ui;

void AIPlayer::init()
{
    mState = std::make_shared<core::GameState>();
}

void AIPlayer::setState(const std::shared_ptr<const core::GameState>& state)
{
    for(auto& obj : *state)
    {
        if(!dynamic_cast<const core::Starship*>(&obj))
        {
            continue;
        }
        auto& own = dynamic_cast<const core::Starship&>(obj);

        if(own.team() != mOwnTeam || !own.alive())
            continue;

        // if the shield is damaged.
        float own_shield = own.shield_strength().current;

        length_t min = 100.0_km;
        // find closest ship to attack
        const core::Starship* target = nullptr;
        for (auto& e : *state)
        {
            auto eship = dynamic_cast<const core::Starship*>(&e);
            if(!eship || !eship->alive())
                continue;

            if (eship->team() != own.team() && distance(e, own) < min) {
                min = distance(e, own);
                target = eship;
            }
        }

        if(!target)
            continue;

        // if found, do attack
        if(min < 10.0_km)  {
            mCommands.addCommand( cmd::Attack(own.id(), target->id()) );
        }

        // fly closer if shield is stronger
        if(own_shield > target->shield_strength().current || own_shield > 2)
        {
            mCommands.addCommand( cmd::Move(own.id(), target->position(), 0.2_kps) );
        } else
        {
            auto delta = own.position() - target->position();
            delta *= 1.0_km / length(delta);
            mCommands.addCommand( cmd::Move(own.id(), own.position() + delta, 1.0_kps) );
        }
    }

    mCommands.validate(*state);
    mState = state;
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
        if(mState->getShip(hit).team() == mOwnTeam)
        {
            // don't do anything for now!
        }
    }
    return false;
}

AIPlayer::AIPlayer(std::uint64_t team) : mOwnTeam( team )
{

}

void AIPlayer::getCommandEvents(std::vector<events::EventPtr>& evts) const
{
    mCommands.transcribe(*mState, evts);
}
