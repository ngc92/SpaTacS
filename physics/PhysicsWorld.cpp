//
// Created by erik on 9/27/16.
//

#include "PhysicsWorld.h"
#include "HitTests.h"
#include "Fixture.h"
#include <boost/optional.hpp>
#include <boost/variant.hpp>
#include <iostream>

using namespace spatacs::physics;
using spatacs::Vec;


namespace
{
    template<class Base>
    struct GetUnderlyingContainer : public Base
    {
        using container_t = typename Base::container_type;
        container_t& container() { return Base::c; }
        const container_t& container() const  { return Base::c; }
    };
}

void PhysicsWorld::pushEvent(Collision evt)
{
    mEventQueue.push( std::move(evt) );
}

const Object& PhysicsWorld::getObject(ObjectID id) const
{
    return getObjectRec(id).object;
}

void PhysicsWorld::setCollisionCallback(PhysicsWorld::collision_callback_fn cb)
{
    mCollisionCallback = std::move(cb);
}

void PhysicsWorld::simulate(time_t time_step)
{
    mNow = 0.0_s;

    // detect all collisions
    for(auto& obj : mObjects)
        detectCollisionsOf(obj.first, time_step);

    // now handle all events
    while( !mEventQueue.empty() )
    {
        auto ev = std::move(mEventQueue.top());
        mEventQueue.pop();

        ImpactInfo info;
        info.time = ev.time;
        info.fixture_A = ev.fA;
        info.fixture_B = ev.fB;
        mNow = ev.time;
        if(ev.time <= time_step) {
            /// \todo maybe we should update A and B here to the ev.time state
            mCollisionCallback(*this, getObject(ev.A), getObject(ev.B), info);
        }
    }

    mNow = time_step;

    // update positions
    for(auto& obr : mObjects)
    {
        auto dt = mNow - obr.second.update;
        obr.second.object.setPosition( obr.second.object.position() + dt * obr.second.object.velocity());
        obr.second.object.setVelocity( obr.second.object.velocity() + obr.second.acceleration );
        obr.second.acceleration = velocity_vec{Vec{0, 0, 0}};
        obr.second.update = 0.0_s;
    }
}

void PhysicsWorld::detectCollisionsOf(ObjectID id, time_t max_dt, bool all)
{
    auto& orec = getObjectRec(id);
    auto& obj = orec.object;
    for(auto& fixa : obj) {
        length_vec original_position = obj.position() - orec.update * obj.velocity();
        MovingSphere ppath = {original_position, obj.velocity(), fixa.radius(), speed_t(0)};
        // check ship impact
        for (auto& target : mObjects) {
            // no self-intersection, check each pair only once
            if (target.first == id || (id < target.first && !all))
                continue;
            /// \todo disable projectile-projectile hit tests
            Object& tobj = target.second.object;
            for (auto& fix : tobj)
            {
                auto opos = tobj.position() - target.second.update * tobj.velocity();
                MovingSphere tpath = {opos, tobj.velocity(), fix.radius(), speed_t(0)};
                auto hit = intersect(tpath, ppath);
                if (hit) {
                    time_t time = hit.get();
                    if (time >= mNow && time < max_dt) {
                        pushEvent(Collision{id, target.first, fixa.userdata(), fix.userdata(), time});
                    }
                }
            }
        }
    }
}

void PhysicsWorld::filterCollisions(ObjectID id)
{
    std::vector<Collision> container = std::move(((GetUnderlyingContainer<decltype(mEventQueue)>&)(mEventQueue)).container());
    auto nend = std::remove_if(begin(container), end(container), [id](auto& e){ return e.A == id || e.B == id;});
    container.resize( std::distance(begin(container), nend) );

    // construct a new queue, because I don't know whether we might hav destroyed some invariants in
    // the process above.
    mEventQueue = queue_t(Compare(), std::move(container));
}


const PhysicsWorld::ObjectRecord& PhysicsWorld::getObjectRec(ObjectID id) const
{
    return mObjects.at(id);
}

PhysicsWorld::ObjectRecord& PhysicsWorld::getObjectRec(ObjectID id)
{
    return mObjects.at(id);
}

ObjectID PhysicsWorld::spawn(const Object& object)
{
    ObjectRecord new_rec;
    new_rec.object = object;
    ObjectID id{mFreeID};
    new_rec.object.setID( id );
    new_rec.acceleration = velocity_vec(Vec{0, 0, 0});
    mObjects[id] = std::move(new_rec);
    ++mFreeID;
    return id;
    /// \todo trigger calculation of collision events.
}

bool PhysicsWorld::despawn(ObjectID id)
{
    auto e = mObjects.erase(id);
    if( e != 1 )
    {
        return false;
    }

    filterCollisions(id);
    return true;
}

void PhysicsWorld::applyForce(ObjectID id, force_vec force)
{
    /// \todo this is a fixed end time! Change!
    time_t application_time = 0.1_s;
    auto& target = getObjectRec(id);
    target.acceleration += force * application_time / target.object.mass();
}

void PhysicsWorld::setMass(ObjectID id, mass_t mass)
{
    auto& target = getObjectRec(id);
    target.object.setMass( mass );
}

void PhysicsWorld::applyImpulse(ObjectID id, impulse_vec impulse)
{
    auto& target = getObjectRec(id);
    auto vel_old = target.object.velocity();
    target.object.setVelocity( vel_old + impulse / target.object.mass() );

    // perform part of the integration now
    auto dt = mNow - target.update;
    target.object.setPosition( target.object.position() + dt * vel_old);
    target.update = mNow;

    // now we can update the collision tests
    // remove old collisions of id
    filterCollisions(id);
    detectCollisionsOf( id, 1.0_s, true );
}

bool PhysicsWorld::Compare::operator()(const Collision& a, const Collision& b) const
{
    return a.time > b.time;
}
