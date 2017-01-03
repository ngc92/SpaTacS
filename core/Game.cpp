#include "Game.h"
#include "GameInterface.h"
#include "SimulationBase.h"
#include "GameStateBase.h"
#include <chrono>
#include "core/detail/GameThread.h"
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
        auto state  = std::shared_ptr<GameStateBase>(mThread->getState());
        assert(state);
        auto events = mThread->getEvents();

        any_vector in_events;
        for(auto& itf : mUIs)
        {
            itf->getCommandEvents(in_events);
        }

        // process events generated due to commands
        mThread->setInEvents( std::move(in_events) );

        for(auto& itf : mUIs)
        {
            itf->setState( state );
            itf->notify( events );
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

