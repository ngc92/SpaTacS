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

void GameSimulation::update()
{
    // get the acceleration of ships and convert to physics events
    for (auto& ship : dynamic_cast<game::GameState&>(*mState)) {
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
    for (auto& ship : dynamic_cast<game::GameState&>(*mState)) {
        auto po = mWorld->getObject(ship.physics_id() );
        ship.setPosition( po.position() );
        ship.setVelocity( po.velocity() );
        ship.setMass( po.mass() );
        ship.onStep();

        if(!ship.alive()) {
            mWorld->despawn(ship.physics_id());
        }
    }
}

void GameSimulation::physics_callback(physics::PhysicsWorld& world, const physics::Object& A,
                                                     const physics::Object& B,
                                                     physics::ImpactInfo info)
{
    uint64_t id_a = A.userdata();
    uint64_t id_b = B.userdata();

    // get objects associated with A and B.
    auto* ob_A = &dynamic_cast<game::GameState*>(mState.get())->getObject( game::ObjectID(id_a) );
    auto* ob_B = &dynamic_cast<game::GameState*>(mState.get())->getObject( game::ObjectID(id_b) );

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
                addEvent(std::make_unique<events::HitShield>(*aship, proj));
            } else {    // ship fixture
                addEvent(std::make_unique<events::Hit>(*aship, proj));
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
    std::vector<EventPtr> events = move(mEventCache);
    while(!events.empty()) {
        for (auto& event : events) {
            events::EventContext ctx{dynamic_cast<game::GameState&>(*mState), mEventCache, *mWorld };
            event->apply(ctx);
        }

        // save all current events in all_events and update event list
        mAllEvents.reserve( mAllEvents.size() + events.size() );
        move( begin(events), end(events), back_inserter(mAllEvents) );
        events = move( mEventCache );
    }
}