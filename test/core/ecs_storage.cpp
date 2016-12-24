//
// Created by erik on 12/24/16.
//


#include "physics/PhysicsWorld.h"
#include <boost/test/unit_test.hpp>
#include <iostream>
#include "core/ecs/System.h"

#include "core/ecs/EntityManager.h"
#include "core/ecs/ComponentStorage.h"
#include "core/ecs/MetaStorage.h"
#include "core/ecs/EntityStorage.h"
#include "core/ecs/EntityHandle.h"

BOOST_AUTO_TEST_SUITE(ECS_tests)

    using namespace spatacs::core;

    BOOST_AUTO_TEST_CASE(ComponentStorage)
    {
        ecs::ComponentStorage<int, float, double> cs;

        cs.resize(10);
        BOOST_CHECK_EQUAL( cs.size(), 10 );

        auto r = cs.get(0);
        std::get<0>(r) = 5;
        std::get<1>(r) = 1.5f;
        std::get<2>(r) = 2.0;
        static_assert( std::is_same<decltype(r), std::tuple<int&, float&, double&>>::value, "Incorrect type." );

        BOOST_CHECK_EQUAL(std::get<0>(cs.get(0)), 5);
        BOOST_CHECK_EQUAL(std::get<1>(cs.get(0)), 1.5f);
        BOOST_CHECK_EQUAL(std::get<2>(cs.get(0)), 2.0);
    }

    struct MockMgr;
    struct MockConfig
    {
        using manager_t = MockMgr;
        using id_t      = spatacs::TaggedID<std::size_t, struct mock_t>;
        using cmp_storage_t = ecs::ComponentStorage<int, float, double>;
        using comp_vec      = pack_t<int, float, double>;
        constexpr static std::size_t comp_count = 5;
    };

    BOOST_AUTO_TEST_CASE(MetadataStorage)
    {
        using cfg_t = ecs::MetaConfig<5, std::size_t , std::size_t>;
        ecs::MetadataStorage<cfg_t> ms;

        ms.resize(10);
        BOOST_CHECK_EQUAL( ms.size(), 10 );
        BOOST_CHECK_EQUAL( ms.getNextFreeIndex(), 0 );

        // Lifetime mgm
        for(int i = 0; i < 10; ++i) {
            BOOST_CHECK_EQUAL(ms.is_alive(i), false);
        }

        ms.create(2, 32);
        BOOST_CHECK_EQUAL(ms.is_alive(2), true);
        BOOST_CHECK_EQUAL(ms.id_of(2), 42);

        ms.kill(2);
        BOOST_CHECK_EQUAL(ms.is_alive(2), false);

        // Bits mgm
        for(int i = 0; i < 10; ++i) {
            BOOST_CHECK_EQUAL(ms.bits(i), 0);
        }

        ms.mutable_bits(0).flip();
        BOOST_CHECK(ms.bits(0).all());

        // kill does not yet change the bits
        ms.kill(0);
        BOOST_CHECK(ms.bits(0).all());
        // ensure that newly creates one have all bits set to zero
        ms.create(0, 42);
        BOOST_CHECK(ms.bits(0).none());
    }

    BOOST_AUTO_TEST_CASE(EntityStorage)
    {
        ecs::EntityStorage<MockConfig> es;
        es.resize(2);
        BOOST_REQUIRE_EQUAL(es.size(), 2);

        auto first = es.create();
        auto second = es.create();
        BOOST_CHECK( es.is_alive(first) );
        BOOST_CHECK( es.is_alive(second) );

        BOOST_CHECK_EQUAL(&es.kill(first), &es);
        BOOST_CHECK(!es.is_alive(first));

        // without proper versioning, we wil overwrite here
        auto third = es.create();
        BOOST_CHECK(es.is_alive(third));
        BOOST_CHECK(!es.is_alive(first));

        // automatic resizing
        auto forth = es.create();
        BOOST_CHECK(es.is_alive(forth));
        BOOST_CHECK_GE(es.size(), 3);


        // bits test
        BOOST_CHECK(es.bits(es.lookup(second)).none());
        es.add_component(second, type_t<double>{}, 1.0);
        BOOST_CHECK_EQUAL(es.bits(es.lookup(second)), std::bitset<5>(0b00100));

        // components test
        std::get<0>(es.mutable_components(es.lookup(second))) = 2;
        BOOST_CHECK_EQUAL( std::get<0>(es.components(es.lookup(second))), 2 );

        es.add_component(third, type_t<int>{}, 1);
        BOOST_CHECK_EQUAL( std::get<0>(es.components(es.lookup(third))), 1 );
        BOOST_CHECK_EQUAL( es.get_component(third, type_t<int>{}), 1 );
        BOOST_CHECK(es.bits(es.lookup(third)).test(0));

        es.add_component(third, type_t<float>{}, 1.f);
        BOOST_CHECK(es.has_component(third, type_t<float>{}));
        es.get_mutable_component(third, type_t<float>{}) = 1.5f;
        BOOST_CHECK_EQUAL( es.get_component(third, type_t<float>{}), 1.5f );

        es.remove_component(third, type_t<float>{});
        BOOST_CHECK(!es.has_component(third, type_t<float>{}));

        // iteration
        auto indices = es.index_range();
        BOOST_CHECK_EQUAL(boost::size(indices), 3);
        es.kill(third);
        indices = es.index_range();
        BOOST_CHECK_EQUAL(boost::size(indices), 2);
    }
BOOST_AUTO_TEST_SUITE_END()
