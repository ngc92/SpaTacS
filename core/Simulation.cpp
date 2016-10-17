//
// Created by erik on 10/1/16.
//

#include <iostream>
#include <events/Combat.h>
#include <events/Spawn.h>
#include "physics/HitTests.h"
#include "GameThread.h"
#include "Simulation.h"
#include "game/Starship.h"
#include "game/Projectile.h"

using namespace spatacs;
using namespace core;
using namespace game;

auto Simulation::step(EventVec inEvents) -> EventVec
{
    mAllEvents.clear();

    // remove dead objects
    for(auto& s : mState)
    {
        if(!s.alive())
            mWorld->pushEvent( physics::events::Despawn{s.physics_id(), 0.0_s } );
    }

    mState.cleanup();

    // process events generated due to commands
    mEventCache = move(inEvents);
    eventLoop();

    // get the acceleration of ships and convert to physics events
    for (auto& ship : mState) {
        if(ship.type() == ObjectType::STARSHIP)
        {
            auto acc = dynamic_cast<game::Starship&>(ship).getProducedAcceleration();
            mWorld->pushEvent( physics::events::ApplyForce{ship.physics_id(), acc * ship.mass(), 0.0_s} );
        }
    }

    // let the physics engine generate its events and process them
    /*  Push external physics events
     */
    mWorld->simulate(0.1_s);
    eventLoop();

    // copy new positions and velocities to game objects
    for (auto& ship : mState) {
        auto po = mWorld->getObject( ship.physics_id() );
        ship.setPosition( po.position() );
        ship.setVelocity( po.velocity() );
    }

    // step all objects
    for (auto& ship : mState) {
        ship.onStep();
    }

    return std::move(mAllEvents);
}

void Simulation::eventLoop()
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

void Simulation::addEvent(EventPtr e)
{
    mEventCache.push_back( move(e) );
}

void Simulation::physics_callback(physics::PhysicsWorld& world, const physics::Object& A,
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

Simulation::Simulation():
        mWorld(std::make_unique<physics::PhysicsWorld>() )
{
    mWorld->setCollisionCallback([this](physics::PhysicsWorld& world,
                                        const physics::Object& A, const physics::Object& B, physics::ImpactInfo info)
                                             {
                                                 this->physics_callback( world, A, B, info );
                                             });

    addEvent(std::make_unique<events::SpawnShip>(1, "SF Predator", "cruiser",   kilometers(0, 0, 0.4)));
    addEvent(std::make_unique<events::SpawnShip>(1, "SF Fearless", "destroyer", kilometers(0, 0.2, -0.4)));
    addEvent(std::make_unique<events::SpawnShip>(2, "ES Lion",     "destroyer", kilometers(22, 4, 2)));
    addEvent(std::make_unique<events::SpawnShip>(2, "ES Wolf",     "destroyer", kilometers(18, 2, -4)));
    addEvent(std::make_unique<events::SpawnShip>(2, "ES Tiger",    "destroyer", kilometers(15, 2, 3)));
    eventLoop();
}

GameState Simulation::extractState()
{
    return GameState(mState);
}
