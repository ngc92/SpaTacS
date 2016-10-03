//
// Created by erik on 9/23/16.
//

#include "AIPlayer.h"
#include <boost/range/adaptor/indirected.hpp>
#include <events/Combat.h>

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
    for(auto& own : state.getShips()) {
        if(own.team() != mOwnTeam)
            continue;
        length_t min = 10.0_km;
        std::uint64_t attack = 0;
        for (auto& e : state.getShips()) {
            if (e.team() != own.team() && distance(e, own) < min) {
                min = distance(e, own);
                attack = e.id();
            }
        }

        if(attack != 0) {
            mCommands.push_back( cmd::Attack(own.id(), attack) );
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
            auto opos = mLastState->getShip(hit).position();
            opos.x += (rand() % 101 - 50.0) * 0.01_km;
            opos.y += (rand() % 101 - 50.0) * 0.01_km;
            opos.z += (rand() % 101 - 50.0) * 0.01_km;
            mCommands.push_back( cmd::Move(hit, opos.x, opos.y, opos.z, 2.0_kps) );
        }
    }
    return false;
}

AIPlayer::AIPlayer(std::uint64_t team) : mOwnTeam( team )
{

}
