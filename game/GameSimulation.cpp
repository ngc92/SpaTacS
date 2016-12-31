//
// Created by erik on 11/15/16.
//

#include "GameSimulation.h"
#include "game/Starship.h"
#include "game/Projectile.h"
#include "game/GameState.h"
#include "events/Combat.h"

using namespace spatacs;
using namespace game;

GameSimulation::GameSimulation():
        mWorld(std::make_unique<physics::PhysicsWorld>() ),
        mState(std::make_unique<GameState>() )
{
    mWorld->setCollisionCallback([this](physics::PhysicsWorld& world,
                                        const physics::Object& A, const physics::Object& B, physics::ImpactInfo info)
                                 {
                                     this->physics_callback( world, A, B, info );
                                     eventLoop();
                                 }
    );
}

void GameSimulation::processInput(EventVec inEvents)
{
    mNotifications.clear();
    std::move(begin(inEvents), end(inEvents), std::back_inserter(mEventQueue));
    eventLoop();
}

auto GameSimulation::update() -> NotifyVec
{
    mState->cleanup();

    // get the acceleration of ships and convert to physics events
    for (auto& ship : *mState) {
        if(ship.type() == ObjectType::STARSHIP)
        {
            auto& conv = dynamic_cast<Starship&>(ship);
            auto acc = conv.getProducedAcceleration();
            mWorld->applyForce(ship.physics_id(), acc * ship.mass());
            mWorld->setMass(ship.physics_id(), conv.getTotalMass());
        }
    }

    // let the physics engine generate its events and process them
    /*  Push external physics events
     */
    mWorld->simulate(0.1_s);
    eventLoop();

    // copy new positions and velocities to game objects
    for (auto& object : *mState)
    {
        if(object.physics_id())
        {
            auto po = mWorld->getObject(object.physics_id());
            object.setPosition(po.position());
            object.setVelocity(po.velocity());
            object.setMass(po.mass());
        }
        object.onStep(*mState);

        if(!object.alive() && object.physics_id())
        {
            mWorld->despawn(object.physics_id());
        }
    }

    return core::any_vector(std::move(mNotifications));
}

void GameSimulation::physics_callback(physics::PhysicsWorld& world, const physics::Object& A,
                                                     const physics::Object& B,
                                                     physics::ImpactInfo info)
{
    uint64_t id_a = A.userdata();
    uint64_t id_b = B.userdata();

    // get objects associated with A and B.
    auto* ob_A = &mState->getObject( game::ObjectID(id_a) );
    auto* ob_B = &mState->getObject( game::ObjectID(id_b) );

    // check if A is a ship
    game::Starship* aship = dynamic_cast<game::Starship*>(ob_A);
    game::Starship* bship = dynamic_cast<game::Starship*>(ob_B);
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
        auto& proj = dynamic_cast<game::Projectile&>(*ob_B);
        if(proj.shooter() != aship->id() && proj.age() < 10.0)
        {
            if(info.fixture_A == 1) // shield fixture
            {
                mEventQueue.push_back(std::make_unique<spatacs::events::HitShield>(*aship, proj));
            } else {    // ship fixture
                mEventQueue.push_back(std::make_unique<spatacs::events::Hit>(*aship, proj));
                proj.expire();
            }
        }
    } else
    {
        // nothing.
    }
}

void GameSimulation::eventLoop()
{
    while(!mEventQueue.empty())
    {
        // process next event
        auto& next = mEventQueue.front();
        spatacs::events::EventContext ctx{*mState, mEventQueue, *mWorld, mNotifications };
        next->apply(ctx);

        mEventQueue.pop_front();
    }
}

const GameState& GameSimulation::getState() const
{
    return *mState;
}

