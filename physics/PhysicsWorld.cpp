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

void PhysicsWorld::pushEvent(events::Event evt)
{
    mEventQueue.push( std::move(evt) );
}

const Object& PhysicsWorld::getObject(std::uint64_t id) const
{
    return getObjectRec(id).object;
}

void PhysicsWorld::setCollisionCallback(PhysicsWorld::collision_callback_fn cb)
{
    mCollisionCallback = std::move(cb);
}

struct PhysicsWorld::HandleEvent : public boost::static_visitor<void>
{
    HandleEvent(PhysicsWorld* world) : world(world)
    {}

    PhysicsWorld* world;
    template<class T>
    void operator()(const T& t) const
    {
        world->handleEvent(t);
    }
};


void PhysicsWorld::simulate(time_t dt)
{
    // detect all collisions
    for(auto& obj : mObjects)
        detectCollisionsOf(obj.first, dt);

    // now handle all events
    while( !mEventQueue.empty() )
    {
        auto ev = std::move(mEventQueue.top());
        mEventQueue.pop();
        boost::apply_visitor( HandleEvent{this}, ev );
    }

    // update positions
    for(auto& obr : mObjects)
    {
        obr.second.object.setPosition( obr.second.object.position() + dt * obr.second.object.velocity());
        obr.second.object.setVelocity( obr.second.object.velocity() + obr.second.acceleration );
        obr.second.acceleration = velocity_vec{Vec{0, 0, 0}};
    }
}

void PhysicsWorld::detectCollisionsOf(std::uint64_t id, time_t max_dt, bool all)
{
    auto& obj = getObject(id);
    for(auto& fixa : obj) {
        MovingSphere ppath = {obj.position(), obj.velocity(), fixa.radius(), speed_t(0)};
        // check ship impact
        for (auto& target : mObjects) {
            // no self-intersection, check each pair only once
            if (target.first == id || (id < target.first && !all))
                continue;
            /// \todo disable projectile-projectile hit tests
            Object& tobj = target.second.object;
            for (auto& fix : tobj) {
                MovingSphere tpath = {tobj.position(), tobj.velocity(), fix.radius(), speed_t(0)};
                auto hit = intersect(tpath, ppath);
                if (hit) {
                    time_t time = hit.get();
                    if (time >= 0.0_s && time < max_dt) {
                        pushEvent(events::Collision{id, target.first, fixa.userdata(), fix.userdata(), time});
                    }
                }
            }
        }
    }
}

void PhysicsWorld::handleEvent(const events::Collision& c)
{
    ImpactInfo info;
    info.time = c.time;
    info.fixture_A = c.fA;
    info.fixture_B = c.fB;
    mCollisionCallback( *this, getObject(c.A), getObject(c.B), info );
}

void PhysicsWorld::handleEvent(const events::Despawn& d)
{
    auto e = mObjects.erase(d.target);
    if( e != 1 )
    {
        BOOST_THROW_EXCEPTION( std::logic_error("Invalid despawn event") );
    }

    /// \todo this still leaves other events which might affect the despawned
    /// object in the queue.
    filterCollisions(d.target);
}

void PhysicsWorld::filterCollisions(std::uint64_t id)
{
    std::vector<events::Event> container = std::move(((GetUnderlyingContainer<decltype(mEventQueue)>&)(mEventQueue)).container());
    auto nend = std::remove_if(begin(container), end(container), [id](auto& e){ return events::has_target_id(id, e);});
    container.resize( std::distance(begin(container), nend) );

    // construct a new queue, because I don't know whether we might hav destroyed some invariants in
    // the process above.
    mEventQueue = queue_t(Compare(), std::move(container));
}

void PhysicsWorld::handleEvent(const events::ApplyForce& f)
{
    /// \todo this is a fixed end time! Change!
    time_t end_time = 0.1_s;
    time_t time_remaining = end_time - f.time;
    auto& target = getObjectRec(f.target);
    /// \todo specify units and mass handling!
    target.acceleration += f.force * time_remaining / target.object.mass();
}


void PhysicsWorld::handleEvent(const events::SetMass& e)
{
    auto& target = getObjectRec(e.target);
    target.object.setMass( e.mass );
}


const PhysicsWorld::ObjectRecord& PhysicsWorld::getObjectRec(std::uint64_t id) const
{
    return mObjects.at(id);
}

PhysicsWorld::ObjectRecord& PhysicsWorld::getObjectRec(std::uint64_t id)
{
    return mObjects.at(id);
}

std::uint64_t PhysicsWorld::spawn(const Object& object)
{
    ObjectRecord new_rec;
    new_rec.object = object;
    new_rec.object.setID(mFreeID);
    new_rec.acceleration = velocity_vec(Vec{0, 0, 0});
    mObjects[mFreeID] = std::move(new_rec);
    std::uint64_t id = mFreeID;
    ++mFreeID;
    return id;
    /// \todo trigger calculation of collision events.
}

bool PhysicsWorld::Compare::operator()(const events::Event& a, const events::Event& b) const
{
    return get_time(a) > get_time(b);
}
