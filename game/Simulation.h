//
// Created by erik on 1/2/17.
//

#ifndef SPATACS_GAME_SIMULATION_H
#define SPATACS_GAME_SIMULATION_H

#include "core/SimulationBase.h"
#include "game/events/notifications.h"

namespace spatacs
{
namespace physics
{
    class PhysicsWorld;
    class Object;
    class ImpactInfo;
}
namespace game
{
    class State;

    class Simulation : public core::SimulationBase
    {
    public:
        Simulation();

        // overridden functions
        void processInput(EventVec inEvents) override;
        NotifyVec update() override;
        const core::GameStateBase& getState() const override;
    private:
        // physics
        std::unique_ptr<physics::PhysicsWorld> mWorld;
        void physics_callback(physics::PhysicsWorld& world, const physics::Object& A,
                              const physics::Object& B,
                              physics::ImpactInfo info);

        // events
        std::vector<events::notification_t> mNotifications;

        // state
        std::unique_ptr<State> mState;
    };
}
}

#endif //SPATACS_GAME_SIMULATION_H
