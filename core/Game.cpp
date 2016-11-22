#include "Game.h"
#include "GameInterface.h"
#include "SimulationBase.h"
#include "GameStateBase.h"
#include <chrono>
#include <algorithm>
#include "core/detail/GameThread.h"
#include "events/Accelerate.h"
#include "events/Combat.h"
#include <iostream>

using namespace spatacs;
using namespace core;

Game::Game(std::unique_ptr<SimulationBase> simulation) :
    mThread( std::make_unique<detail::GameThread>( std::move(simulation) ) )
{
}

Game::~Game()
{

}

void Game::run()
{
    // step all user interfaces
    for(auto& itf : mUIs)
    {
        itf->step();
    }

    // does the game thread have new data for us?
    if( mThread->has_data() )
    {
        // if yes, we update stuff
        mState  = std::shared_ptr<GameStateBase>(mThread->getState());
        mEvents = mThread->getEvents();

        std::vector<events::EventPtr> in_events;
        for(auto& itf : mUIs)
        {
            itf->getCommandEvents(in_events);
        }

        // shuffle commands.
        std::shuffle(begin(in_events), end(in_events), mRandom);

        // process events generated due to commands
        mThread->setInEvents( std::move(in_events) );

        for(auto& itf : mUIs)
        {
            itf->setState( mState );
            itf->notifyEvents( mEvents );
        }
    }
}

void Game::addInterface(std::shared_ptr<core::GameInterface> ui)
{
    ui->init();
    mUIs.push_back( std::move(ui) );
}

void Game::setPause(bool p)
{
    mThread->setPause(p);
}

