//
// Created by erik on 1/2/17.
//

#include "Simulation.h"
#include "State.h"
#include "physics/PhysicsWorld.h"
#include "entity/entity.h"

#include "entity/systems/Physics.h"
#include "events/commands.h"

using namespace spatacs;
using namespace game;

Simulation::Simulation():
        mWorld(std::make_unique<physics::PhysicsWorld>() ),
        mState(std::make_unique<State>() )
{
    mWorld->setCollisionCallback([this](physics::PhysicsWorld& world,
                                        const physics::Object& A, const physics::Object& B, physics::ImpactInfo info)
                                 {
                                     this->physics_callback( world, A, B, info );
                                 }
    );
}

const core::GameStateBase& Simulation::getState() const
{
    return *mState;
}

auto Simulation::update() -> NotifyVec
{
    /// \todo state cleanup

    // push all physics updates into the world.
    mState->entity_manager().apply(systems::UpdateToWorld{}, *mWorld);

    // let the physics engine generate its events and process them
    mWorld->simulate(0.1_s);

    // update the entities from the new world data.
    mState->entity_manager().apply(systems::UpdateFromWorld{}, *mWorld);

    return core::any_vector(std::move(mNotifications));
}

void Simulation::processInput(EventVec inEvents)
{
    // reset the notifications
    mNotifications.clear();

    // process all the commands
    auto& commands = inEvents.get_as<events::commands_t>();
}

void Simulation::physics_callback(physics::PhysicsWorld& world, const physics::Object& A,
                                      const physics::Object& B,
                                      physics::ImpactInfo info)
{
    uint64_t id_a = A.userdata();
    uint64_t id_b = B.userdata();
}
