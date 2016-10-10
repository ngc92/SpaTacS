#include "Game.h"
#include "GameState.h"
#include "UI/IUI.h"
#include <chrono>
#include <algorithm>
#include "GameThread.h"
#include "events/Accelerate.h"
#include "events/Combat.h"
#include "components/Engine.h"
#include <iostream>
#include "core/components/IWeapon.h"

using namespace spatacs;
using namespace core;

namespace
{
    using EventPtr = events::EventPtr;

    struct CommandVisitor : public boost::static_visitor<void>
    {
        CommandVisitor(GameState *pState) : state( pState ){

        }

        template<class T>
        void operator()(const T& cmd);
        GameState* state;
        std::vector<EventPtr> events;
    };
}

Game::Game()
{
    mThread.reset( new GameThread );
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
        mState  = mThread->getState();
        mEvents = mThread->getEvents();
        // get our own events to put into the thread
        // first, gather the commands
        std::vector<cmd::Command> commands;
        for(auto& itf : mUIs) {
            for (auto& c : itf->getCommands())
                commands.push_back( c );
        }

        // then transform them into events
        // shuffle commands.
        std::shuffle(begin(commands), end(commands), mRandom);

        CommandVisitor visit(&mState);
        for (const auto &command : commands) {
            try {
                boost::apply_visitor(visit, command);
            } catch (std::exception& ex)
            {
                std::cerr << "Could not process command "
                          << command << ": "
                          << ex.what() << "\n";
            }
        }

        // process events generated due to commands
        mThread->setInEvents( std::move(visit.events) );

        for(auto& itf : mUIs)
        {
            itf->setState( mState );
            itf->notifyEvents( mEvents );
        }
    }
}

void Game::addInterface(std::shared_ptr<ui::IUI> ui)
{
    ui->init();
    mUIs.push_back( std::move(ui) );
}

namespace {
    template<class T>
    void CommandVisitor::operator()(const T& command)
    {
        std::cout << "unknown command " << command << "\n";
    }

    template<>
    void CommandVisitor::operator()(const cmd::Move& command)
    {
        auto& ship = state->getShip(command.object());
        if(!ship.alive())
            return;

        // and handle the command
        physics::time_t time_to_brake = length(ship.velocity()) / (ship.engine().max_thrust() / ship.mass());

        auto delta = (command.target() - (ship.position() + time_to_brake * ship.velocity())) / 1.0_s;
        auto l = length(delta);
        if(l > command.speed())
            delta *= double(command.speed() / l);

        /// \todo this looks fishy!
        auto dv = (delta - ship.velocity()) / 1.0_s;

        events.push_back(EventPtr(new events::Accelerate(ship.id(), dv)));
    }

    template<>
    void CommandVisitor::operator()(const cmd::Attack& command)
    {
        auto& ship   = state->getShip(command.object());
        auto& target = state->getShip(command.target());
        /// \todo prevent deliberate friendly fire

        // dead ships don't shoot
        if(!ship.alive() || !target.alive())
            return;

        std::size_t wp_count = ship.weapon_count();
        for(std::size_t i = 0; i < wp_count; ++i)
        {
            if(ship.weapon(i).ready())
                events.push_back( EventPtr(new events::FireWeapon(ship.id(), target.id(), i)) );
        }

    }

    template<>
    void CommandVisitor::operator()(const cmd::SetWpnMode& command)
    {
        auto& ship   = state->getShip(command.object());

        // dead ships don't shoot
        if(!ship.alive())
            return;

        events.push_back( EventPtr(new events::SetWeaponMode(ship.id(), command.weapon(), command.mode())) );
    }
}


