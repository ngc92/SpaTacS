//
// Created by erik on 11/2/16.
//

#include "physics/PhysicsWorld.h"
#include <boost/test/unit_test.hpp>
#include <iostream>


using namespace spatacs::physics;

BOOST_AUTO_TEST_SUITE(Fixture_tests)
    BOOST_AUTO_TEST_CASE(Constructor)
    {
        // This test checks that the constructor initializes all values properly
        ObjectID parent{5};
        length_t radius = 0.1_m;
        Fixture f(parent, FixtureID{1}, radius);

        BOOST_CHECK_EQUAL(f.parent(), parent);
        BOOST_CHECK_EQUAL(f.radius(), radius);
        BOOST_CHECK_EQUAL(f.id(), FixtureID{1});
    }

    BOOST_AUTO_TEST_CASE(GetSet)
    {
        // This test checks that getters and setters work as expected
        ObjectID parent{5};
        length_t radius = 0.1_m;
        Fixture f(parent, FixtureID{1}, radius);

        radius = 1.0_m;
        f.setRadius(radius);
        BOOST_CHECK_EQUAL(f.radius(), radius);
        BOOST_CHECK_THROW(f.setRadius(-1.0_m), std::logic_error);

        f.setUserdata(10ul);
        BOOST_CHECK_EQUAL(f.userdata(), 10ul);

        ++parent;
        f.setParent(parent);
        BOOST_CHECK_EQUAL(f.parent(), parent);
    }

BOOST_AUTO_TEST_SUITE_END()



BOOST_AUTO_TEST_SUITE(Object_tests)
    BOOST_AUTO_TEST_CASE(Constructor)
    {
        // This test checks that the constructor initializes all values properly
        length_vec p{5.0_m, 1.0_m, 4.8_m};
        velocity_vec v{1.0_kps, 4.5_kps, -2.5_kps};
        mass_t m = 12.5_kg;
        std::uint64_t ud = 42;

        Object o{p, v, m, ud};

        BOOST_CHECK_EQUAL(o.position(), p);
        BOOST_CHECK_EQUAL(o.velocity(), v);
        BOOST_CHECK_EQUAL(o.mass(), m);
        BOOST_CHECK_EQUAL(o.userdata(), ud);
        BOOST_CHECK(!(bool)o.id());
        BOOST_CHECK(o.begin() == o.end());
    }

    BOOST_AUTO_TEST_CASE(GetSet)
    {
        // This test checks that getters and setters work as expected
        length_vec p{5.0_m, 1.0_m, 4.8_m};
        velocity_vec v{1.0, 4.5, -2.5};
        mass_t m = 12.5_kg;
        std::uint64_t ud = 42;

        Object o;

        o.setPosition(p);
        BOOST_CHECK_EQUAL(o.position(), p);

        o.setVelocity(v);
        BOOST_CHECK_EQUAL(o.velocity(), v);

        o.setMass(m);
        BOOST_CHECK_EQUAL(o.mass(), m);

        o.setUserdata(ud);
        BOOST_CHECK_EQUAL(o.userdata(), ud);

        BOOST_CHECK_THROW(o.setID(ObjectID{}), std::logic_error);

        o.setID(ObjectID{12});
        BOOST_CHECK_EQUAL(o.id(), ObjectID{12});

        BOOST_CHECK_THROW(o.setID(ObjectID{12}), std::logic_error);
    }

    BOOST_AUTO_TEST_CASE(Fixtures)
    {
        // This test checks the functions for adding, removing and iterating fixtures
        Object o;

        auto& f1 = o.addFixture( 0.5_m );
        // check that there is a fixture
        BOOST_CHECK_EQUAL(&f1, &(*o.begin()));
        BOOST_CHECK_EQUAL(o.end() - o.begin(), 1);
        BOOST_CHECK_EQUAL(f1.radius(), 0.5_m);
        f1.setUserdata(42);
        BOOST_CHECK_EQUAL(o.begin()->userdata(), 42);

        // check that the ID is correctly updated
        BOOST_CHECK_EQUAL(f1.parent(), o.id());
        o.setID( ObjectID{5} );
        BOOST_CHECK_EQUAL(f1.parent(), o.id());

        // second fixture
        auto& f2 = o.addFixture( 0.25_m );
        BOOST_CHECK_EQUAL(o.end() - o.begin(), 2);
        BOOST_CHECK_EQUAL(f2.parent(), o.id());

        BOOST_CHECK(f1.id() != f2.id());
    }

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(PhysicsWorld_tests)
    BOOST_AUTO_TEST_CASE(Errors)
    {
        // create an empty world
        PhysicsWorld world;

        // check that we cannot get an Object that does not exist.
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