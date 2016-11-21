//
// Created by erik on 10/1/16.
//

#ifndef SPATACS_CORE_SIMULATION_BASE_H
#define SPATACS_CORE_SIMULATION_BASE_H

#include <events/IEvent.h>
#include "game/GameState.h"
#include "physics/PhysicsWorld.h"

namespace spatacs
{
namespace core
{
    class SimulationBase
    {
        using EventPtr = std::unique_ptr<events::IEvent>;
        using EventVec = std::vector<EventPtr>;
        using StatePtr = std::unique_ptr<GameStateBase>;

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
        game::GameState mState;
        std::unique_ptr<physics::PhysicsWorld> mWorld;

        void addEvent(EventPtr e );
    public:
        SimulationBase();
        virtual ~SimulationBase() = default;

        EventVec step(EventVec inEvents);
        StatePtr extractState() const;
    };
}
}


#endif //SPATACS_CORE_SIMULATION_BASE_H
