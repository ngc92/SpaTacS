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
    class SimulationBase
    {
        using EventPtr = std::unique_ptr<events::IEvent>;
        using EventVec = std::vector<EventPtr>;

        // thread internal variables, don't touch from outside!
        EventVec mEventCache;
        EventVec mAllEvents;

        void eventLoop();
        void cleanup();

        virtual void physics_callback(physics::PhysicsWorld& world, const physics::Object& A, const physics::Object& B,
                              physics::ImpactInfo info) = 0;
        virtual void updateObjects() = 0;
        virtual void updateShips()   = 0;

    protected:
        // thread internal variables, don't touch from outside!
        GameState mState;
        std::unique_ptr<physics::PhysicsWorld> mWorld;

        void addEvent(EventPtr e );
    public:
        SimulationBase();
        virtual ~SimulationBase() = default;

        EventVec step(EventVec inEvents);
        GameState extractState();
    };
}
}


#endif //SOI_SIMULATION_H
