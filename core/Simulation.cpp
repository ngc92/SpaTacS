//
// Created by erik on 10/1/16.
//

#include <iostream>
#include <events/Combat.h>
#include <events/Spawn.h>
#include "physics/HitTests.h"
#include "GameThread.h"
#include "Simulation.h"

using namespace spatacs;

auto core::Simulation::step(EventVec inEvents) -> EventVec
{
    mAllEvents.clear();

    // remove dead objects
    for(auto& s : mState.getShips())
    {
        if(!s.alive())
            mWorld->pushEvent( physics::events::Despawn{s.physics_id(), 0.0_s } );
    }

    for(auto& s : mState.getProjectiles())
    {
        if(s.age() > 10)
            mWorld->pushEvent(physics::events::Despawn{s.physics_id(), 0.0_s});
    }

    mState.getShips().erase(remove_if(begin(mState.getShips()), end(mState.getShips()),
                                      [](const Starship& s) { return !s.alive(); }),
                            end(mState.getShips()));

    mState.getProjectiles().erase(remove_if(begin(mState.getProjectiles()), end(mState.getProjectiles()),
                                            [](const Projectile& s) { return s.age() > 10; }),
                                  end(mState.getProjectiles()));

    // process events generated due to commands
    mEventCache = move(inEvents);
    eventLoop();

    // let the physics engine generate its events and process them
    /*  Push external physics events
     */
    mWorld->simulate(0.1_s);
    eventLoop();

    // copy new positions and velocities to game objects
    for (auto& ship : mState.getShips()) {
        auto po = mWorld->getObject( ship.physics_id() );
        ship.setPosition( po.position() );
        ship.setVelocity( po.velocity() );
    }
    for (auto& proj : mState.getProjectiles()) {
        auto po = mWorld->getObject( proj.physics_id() );
        proj.setPosition( po.position() );
        proj.setVelocity( po.velocity() );
    }

    // step all projectiles and objects
    for (auto& proj : mState.getProjectiles()) {
        proj.onStep();
    }

    for (auto& ship : mState.getShips()) {
        ship.onStep();
    }

    return std::move(mAllEvents);
}

void core::Simulation::eventLoop()
{
    std::vector<GameThread::EventPtr> events = move(mEventCache);
    while(!events.empty()) {
        for (auto& event : events) {
            events::EventContext ctx{mState, mEventCache, *mWorld };
            event->apply(ctx);
        }

        // save all current events in all_events and update event list
        mAllEvents.reserve( mAllEvents.size() + events.size() );
        move( begin(events), end(events), back_inserter(mAllEvents) );
        events = move( mEventCache );
    }
}

void core::Simulation::addEvent(EventPtr e)
{
    mEventCache.push_back( move(e) );
}

void core::Simulation::physics_callback(physics::PhysicsWorld& world, const physics::Object& A,
                                        const physics::Object& B,
                                        physics::ImpactInfo info)
{
    uint64_t id_a = A.userdata();
    uint64_t id_b = B.userdata();

    // get objects associated with A and B.
    auto* ob_A = &mState.getObject( id_a );
    auto* ob_B = &mState.getObject( id_b );

    // check if A is a ship
    Starship* aship = dynamic_cast<Starship*>(ob_A);
    Starship* bship = dynamic_cast<Starship*>(ob_B);
    if(bship) {
        std::swap(aship, bship);
        std::swap(ob_A, ob_B);
        std::swap(info.fixture_A, info.fixture_B);
    }

    // Ship - Ship collision
    if(aship && bship)
    {

    } else if( aship) // Ship - Projectile
    {
        /// \todo use correct positions here!
        auto& proj = dynamic_cast<Projectile&>(*ob_B);
        if(proj.shooter() != aship->id() && proj.age() < 10.0)
        {
            if(info.fixture_A == 1) // shield fixture
            {
                addEvent(std::make_unique<events::HitShield>(*aship, proj));
                eventLoop();
            } else {    // ship fixture
                addEvent(std::make_unique<events::Hit>(*aship, proj));
                eventLoop();
                proj.expire();
            }
        }
    } else
    {
        // nothing.
    }
}

core::Simulation::Simulation():
        mWorld(std::make_unique<physics::PhysicsWorld>() )
{
    mWorld->setCollisionCallback([this](physics::PhysicsWorld& world,
                                        const physics::Object& A, const physics::Object& B, physics::ImpactInfo info)
                                             {
                                                 this->physics_callback( world, A, B, info );
                                             });

    addEvent(std::make_unique<events::SpawnShip>(1, "SF Predator", "destroyer", kilometers(0, 0, 0.4)));
    addEvent(std::make_unique<events::SpawnShip>(1, "SF Fearless", "destroyer", kilometers(0, 0.2, -0.4)));
    addEvent(std::make_unique<events::SpawnShip>(2, "ES Lion",     "destroyer", kilometers(12, 2, 1)));
    addEvent(std::make_unique<events::SpawnShip>(2, "ES Wolf",     "destroyer", kilometers(10, 1, -2)));
    addEvent(std::make_unique<events::SpawnShip>(2, "ES Tiger",    "destroyer", kilometers(11, 2, 3)));
    eventLoop();
}

core::GameState core::Simulation::extractState()
{
    return core::GameState(mState);
}
