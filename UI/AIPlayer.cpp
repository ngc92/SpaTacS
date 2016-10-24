//
// Created by erik on 9/23/16.
//

#include "AIPlayer.h"
#include <boost/range/adaptor/indirected.hpp>
#include <events/Combat.h>
#include <iostream>
#include "game/Starship.h"
#include "core/GameState.h"
#include "UI/cmd/CommandManager.h"

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
        if(!dynamic_cast<const game::Starship*>(&obj))
        {
            continue;
        }
        auto& own = dynamic_cast<const game::Starship&>(obj);

        if(own.team() != mOwnTeam || !own.alive())
            continue;

        // if the shield is damaged.
        double own_shield = own.shield();

        length_t min = 100.0_km;
        // find closest ship to attack
        const game::Starship* target = nullptr;
        for (auto& e : *state)
        {
            auto eship = dynamic_cast<const game::Starship*>(&e);
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
        if(min < 20.0_km)  {
            /// \todo need to react when out of ammo!
            mCommands->addCommand( own.id(), cmd::Attack(target->id()) );
            std::string mode = "HE-light";
            if( target->shield() > 2.0 )
            {
                mode = "SO-light";
            } else if(target->hull_status().current > 2.0)
            {
                mode = "AP-light";
            }

            mCommands->addCommand( own.id(), cmd::SetWpnMode(own.id(), 0, mode) );
        }

        // fly closer if shield is stronger
        if(own_shield > target->shield() || own_shield > 2)
        {
            mCommands->addCommand( own.id(), cmd::Move(target->position(), 0.3_kps));
        } else
        {
            auto delta = own.position() - target->position();
            delta *= 1.0_km / length(delta);
            mCommands->addCommand( own.id(), cmd::Move(own.position() + delta, 2.0_kps));
        }
    }

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

AIPlayer::AIPlayer(std::uint64_t team, std::shared_ptr<cmd::CommandManager> cmd) :
        mOwnTeam( team ),
        mCommands( std::move(cmd) )
{
}
