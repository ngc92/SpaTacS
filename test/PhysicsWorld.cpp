//
// Created by erik on 11/2/16.
//

#include "physics/PhysicsWorld.h"
#include <boost/test/unit_test.hpp>
#include <iostream>


using namespace spatacs::physics;

BOOST_AUTO_TEST_SUITE(PhysicsWorld_tests)
    BOOST_AUTO_TEST_CASE(Errors)
    {
        PhysicsWorld world;
        BOOST_CHECK_THROW(world.getObject( ObjectID{5}), std::logic_error );
        BOOST_CHECK_NO_THROW(world.simulate(0.1_s));
    }

    BOOST_AUTO_TEST_CASE(Spawn)
    {
        PhysicsWorld world;
        Object obj;
        obj.setMass( 100.5_kg );

        auto spawned = world.spawn(obj);
        BOOST_REQUIRE(spawned);

        auto o2 = world.getObject(spawned);
        BOOST_CHECK_EQUAL( spawned, o2.id() );
        BOOST_CHECK_EQUAL( o2.mass(), obj.mass() );

        BOOST_CHECK_THROW(world.getObject( ObjectID{5}), std::logic_error );

        world.despawn(spawned);
        BOOST_CHECK_THROW(world.getObject( spawned ), std::logic_error );
    }

    BOOST_AUTO_TEST_CASE(SetMass)
    {
        PhysicsWorld world;
        Object obj;
        auto spawned = world.spawn(obj);
        world.setMass(spawned, 250.0_kg);

        BOOST_CHECK_EQUAL( world.getObject(spawned).mass(), 250.0_kg );
    }

    BOOST_AUTO_TEST_CASE(ApplyImpulse_Direct)
    {
        PhysicsWorld world;
        Object obj{length_vec{0,0,0}, velocity_vec{0,0,0}, 1.0_kg};
        auto spawned = world.spawn(obj);
        world.applyImpulse(spawned, impulse_vec{1, 0, 0});

        BOOST_CHECK_EQUAL( world.getObject(spawned).velocity(), (velocity_vec{1, 0, 0}) );

        world.simulate(1.0_s);
        BOOST_CHECK_EQUAL( world.getObject(spawned).position(), (length_vec{1, 0, 0}) );
    }

    BOOST_AUTO_TEST_CASE(CollisionCallback)
    {
        int called = 0;
        spatacs::physics::time_t ctime = 0.0_s;
        auto callback = [&](PhysicsWorld& world, const Object& A,
                            const Object& B, ImpactInfo info){ ++called; ctime = info.time; };
        PhysicsWorld world;
        world.setCollisionCallback(callback);

        Object obj{length_vec{0,0,0}, velocity_vec{0,0,0}, 1.0_kg};
        obj.addFixture(1.0_m);
        auto spawned = world.spawn(obj);
        BOOST_REQUIRE(spawned);
        world.simulate(0.1_s);
        BOOST_REQUIRE_EQUAL(called, 0);

        Object o2{length_vec{0,0,2}, velocity_vec{0,0,-2}, 1.0_kg};
        o2.addFixture(0.0_m);
        spawned = world.spawn(o2);
        BOOST_REQUIRE(spawned);
        world.simulate(1.0_s);
        BOOST_CHECK_EQUAL(called, 1);
        BOOST_CHECK_EQUAL(ctime, 0.5_s);
    }

    BOOST_AUTO_TEST_CASE(ApplyImpulse_MidStep)
    {
        PhysicsWorld world;
        Object obj{length_vec{0,0,0}, velocity_vec{1,0,0}, 1.0_kg};
        obj.addFixture(1.0_m);
        auto spawned = world.spawn(obj);
        int called = 0;
        spatacs::physics::time_t ip_time = 0.0_s;
        auto cb = [&](PhysicsWorld& world, const Object& A,
                      const Object& B, ImpactInfo info){
            ++called;
            if(called == 1) {
                world.applyImpulse(spawned, impulse_vec{1, 0, 0});
            } else
            {
                ip_time = info.time;
            }
        };
        world.setCollisionCallback(cb);

        // we cannot time event directly, so use a fake collision to do the timing
        {
            Object o1{length_vec{0, 0, 100}, velocity_vec{1, 0, 0}, 1.0_kg};
            o1.addFixture(1.0_m);
            world.spawn(o1);

            Object o2{length_vec{2.5, 0, 100}, velocity_vec{0, 0, 0}, 1.0_kg};
            o2.addFixture(1.0_m);
            world.spawn(o2);

            // these collide at .5s, this is our timer

            Object o3{length_vec{3, 0, 0}, velocity_vec{0, 0, 0}, 1.0_kg};
            o3.addFixture(1.0_m);
            world.spawn(o3);
        }

        world.simulate(1.0_s);
        BOOST_CHECK_EQUAL( world.getObject(spawned).position(), (length_vec{1.5, 0, 0}) );
        BOOST_REQUIRE_EQUAL(called, 2);
        BOOST_CHECK_EQUAL(ip_time, 0.75_s);

        /// \todo make an automatic test here, comparing a two-step simulation result to a single step simulation where
        /// the impulse is applied in the middle.
    }

BOOST_AUTO_TEST_SUITE_END()