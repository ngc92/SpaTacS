//
// Created by erik on 10/1/16.
//

#include <iostream>
#include <events/Spawn.h>
#include "physics/HitTests.h"
#include "SimulationBase.h"

using namespace spatacs;
using namespace core;
using namespace game;

using EventPtr = std::unique_ptr<events::IEvent>;

auto SimulationBase::step(EventVec inEvents) -> EventVec
{
    cleanup();

    // process events generated due to commands
    mEventCache = move(inEvents);
    eventLoop();
    updateShips();

    // let the physics engine generate its events and process them
    /*  Push external physics events
     */
    mWorld->simulate(0.1_s);
    eventLoop();
    updateObjects();

    return std::move(mAllEvents);
}

void SimulationBase::cleanup()
{
    mAllEvents.clear();

    // remove dead objects
    for(auto& s : mState)
    {
        if(!s.alive())
            mWorld->despawn(s.physics_id() );
    }

    mState.cleanup();
}

void SimulationBase::eventLoop()
{
    std::vector<EventPtr> events = move(mEventCache);
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

void SimulationBase::addEvent(EventPtr e)
{
    mEventCache.push_back( move(e) );
}

SimulationBase::SimulationBase():
        mWorld(std::make_unique<physics::PhysicsWorld>() )
{
    mWorld->setCollisionCallback([this](physics::PhysicsWorld& world,
            const physics::Object& A, const physics::Object& B, physics::ImpactInfo info)
                 {
                     this->physics_callback( world, A, B, info );
                     eventLoop();
                 }
    );
}

GameState SimulationBase::extractState()
{
    return GameState(mState);
}
