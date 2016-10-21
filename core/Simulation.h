//
// Created by erik on 10/1/16.
//

#ifndef SOI_SIMULATION_H
#define SOI_SIMULATION_H

#include <events/IEvent.h>
#include "GameState.h"
#include "physics/PhysicsWorld.h"

namespace spatacs
{
namespace core
{
    class Simulation
    {
        using EventPtr = std::unique_ptr<events::IEvent>;
        using EventVec = std::vector<EventPtr>;
        // thread internal variables, don't touch from outside!
        GameState mState;
        EventVec mEventCache;
        EventVec mAllEvents;
        std::unique_ptr<physics::PhysicsWorld> mWorld;

        void eventLoop();

        void addEvent(EventPtr e );

        void physics_callback(physics::PhysicsWorld& world, const physics::Object& A, const physics::Object& B,
                              physics::ImpactInfo info);

    public:
        Simulation();
        EventVec step(EventVec inEvents);

        GameState extractState();
    };
}
}


#endif //SOI_SIMULATION_H
