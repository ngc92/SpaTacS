//
// Created by erik on 1/2/17.
//

#include "Simulation.h"
#include "State.h"
#include "physics/PhysicsWorld.h"

using namespace spatacs;
using namespace game;

Simulation::Simulation():
        mWorld(std::make_unique<physics::PhysicsWorld>() ),
        mState(std::make_unique<State>() )
{
}

const core::GameStateBase& Simulation::getState() const
{
    return *mState;
}

auto Simulation::update() -> NotifyVec
{
    /// \todo state cleanup
    // let the physics engine generate its events and process them
    mWorld->simulate(0.1_s);

    return core::any_vector(std::move(mNotifications));
}

void Simulation::processInput(EventVec inEvents)
{
    // process all the events

}
