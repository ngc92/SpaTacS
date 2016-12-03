//
// Created by erik on 11/19/16.
//


#include "physics/PhysicsWorld.h"
#include <boost/test/unit_test.hpp>
#include <iostream>
#include <core/V2/System.h>

#include "core/V2/EntityManager.h"
#include "core/V2/ComponentStorage.h"
#include "core/V2/MetaStorage.h"
#include "core/V2/EntityStorage.h"
#include "core/V2/EntityHandle.h"

namespace ecs = spatacs::core::ecs;

BOOST_AUTO_TEST_SUITE(ECS_tests)

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
        using id_t      = std::size_t;
        using cmp_storage_t = ecs::ComponentStorage<int, float, double>;
        using comp_vec      = ecs::type_vec_t<int, float, double>;
        constexpr static std::size_t comp_count = 5;
    };

    BOOST_AUTO_TEST_CASE(MetadataStorage)
    {
        ecs::MetadataStorage<5> ms;

        ms.resize(10);
        BOOST_CHECK_EQUAL( ms.size(), 10 );
        BOOST_CHECK_EQUAL( ms.getNextFreeIndex(), 0 );

        // Lifetime mgm
        for(int i = 0; i < 10; ++i) {
            BOOST_CHECK_EQUAL(ms.is_alive(i), false);
        }

        ms.create(2);
        BOOST_CHECK_EQUAL(ms.is_alive(2), true);

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
        ms.create(0);
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
        BOOST_CHECK(es.bits(second).none());
        es.add_component(second, ecs::type_t<double>{}, 1.0);
        BOOST_CHECK_EQUAL(es.bits(es.lookup(second)), std::bitset<5>(0b00100));

        // components test
        std::get<0>(es.mutable_components(es.lookup(second))) = 2;
        BOOST_CHECK_EQUAL( std::get<0>(es.components(es.lookup(second))), 2 );

        es.add_component(third, ecs::type_t<int>{}, 1);
        BOOST_CHECK_EQUAL( std::get<0>(es.components(es.lookup(third))), 1 );
        BOOST_CHECK_EQUAL( es.get_component(third, ecs::type_t<int>{}), 1 );
        BOOST_CHECK(es.bits(es.lookup(third)).test(0));

        es.add_component(third, ecs::type_t<float>{}, 1.f);
        BOOST_CHECK(es.has_component(third, ecs::type_t<float>{}));
        es.get_mutable_component(third, ecs::type_t<float>{}) = 1.5f;
        BOOST_CHECK_EQUAL( es.get_component(third, ecs::type_t<float>{}), 1.5f );

        es.remove_component(third, ecs::type_t<float>{});
        BOOST_CHECK(!es.has_component(third, ecs::type_t<float>{}));

        // iteration
        auto indices = es.index_range();
        BOOST_CHECK_EQUAL(boost::size(indices), 3);
        es.kill(third);
        indices = es.index_range();
        BOOST_CHECK_EQUAL(boost::size(indices), 2);
    }


    struct MockMgr
    {
        MockMgr(std::size_t size) :
                mAlive(size, false),
                mHasInt(size, false),
                mHasFloat(size, false),
                mIntValues(size, 0),
                mFloatValues(size, 0.0)
        {

        };

        bool is_alive(std::size_t id) const { return mAlive[id]; }
        void kill(std::size_t id) { mAlive[id] = false; }

        template<class T>
        bool has_component(id_t id)
        {
            return std::is_same<T, int>::value ? mHasInt[id] : mHasFloat[id];
        }

        template<class T>
        T& add_component(id_t id, T&& t)
        {
            if(std::is_same<T, int>::value) {
                mHasInt.at(id) = true;
                return reinterpret_cast<T&>(mIntValues.at(id) = t);
            }
            mHasFloat.at(id) = true;
            return  reinterpret_cast<T&>(mFloatValues.at(id) = t);
        }

        template<class T>
        const T& get_component(id_t id) const
        {
            if(std::is_same<T, int>::value) return reinterpret_cast<T&>(mIntValues.at(id));
            return  reinterpret_cast<T&>(mFloatValues.at(id));
        }

        template<class T>
        T& get_mutable_component(id_t id)
        {
            if(std::is_same<T, int>::value) return reinterpret_cast<T&>(mIntValues.at(id));
            return  reinterpret_cast<T&>(mFloatValues.at(id));
        }

        std::vector<bool> mAlive;
        std::vector<bool> mHasInt;
        std::vector<bool> mHasFloat;

        std::vector<int>  mIntValues;
        std::vector<float> mFloatValues;
    };

    BOOST_AUTO_TEST_CASE(EntityHandle)
    {
        /// \todo add test for remove
        using Handle = ecs::EntityHandle<MockConfig>;

        // default construct
        Handle u;
        BOOST_CHECK_EQUAL(u.id(), 0);

        MockMgr mgr(5);
        // initialized handle
        Handle h{mgr, 1};
        BOOST_CHECK_EQUAL(h.id(), 1);

        // aliveness
        BOOST_CHECK_EQUAL(h.alive(), false);
        mgr.mAlive[1] = true;
        BOOST_CHECK_EQUAL(h.alive(), true);
        h.kill();
        BOOST_CHECK_EQUAL(h.alive(), false);

        // components
        BOOST_CHECK_EQUAL(h.has<int>(), false);
        BOOST_CHECK_EQUAL(h.has<float>(), false);
        mgr.mHasInt[1] = true;
        BOOST_CHECK_EQUAL(h.has<int>(), true);


        h.add<float>(5.f);
        BOOST_CHECK_EQUAL(h.has<float>(), true);
        BOOST_CHECK_EQUAL(h.get<float>(), 5.f);

        h.get<int>() = 2;
        BOOST_CHECK_EQUAL(h.get<int>(), 2);
    }

    BOOST_AUTO_TEST_CASE(Signature_SingleParam)
    {
        using sig_t  = ecs::Signature<double>;
        using data_t = std::tuple<bool, int, float, double>;

        auto double_f = [](double d) { BOOST_CHECK_EQUAL(d, 1.2); };
        data_t data {true, 2, 5.2f, 1.2};

        auto double_fw = sig_t::forwarder(double_f);
        double_fw(data);
    }

    BOOST_AUTO_TEST_CASE(Signature_MutliParam)
    {
        using sig_t  = ecs::Signature<double, bool>;
        using data_t = std::tuple<bool, int, float, double>;

        auto multi_f = [](double d, bool b) {
            BOOST_CHECK_EQUAL(d, 1.2);
            BOOST_CHECK(b);
        };
        data_t data {true, 2, 5.2f, 1.2};

        auto double_fw = sig_t::forwarder(multi_f);
        double_fw(data);
    }

    BOOST_AUTO_TEST_CASE(EntityManager)
    {
        using Config = ecs::Config<std::size_t, int, double, float>;
        using EM_t   = ecs::EntityManager<Config>;

        EM_t mgr;

        // lifetime
        BOOST_CHECK(!mgr.is_alive(0));
        BOOST_CHECK(!mgr.is_alive(1));

        auto first = mgr.create();
        BOOST_CHECK_EQUAL(first.id(), 1);
        BOOST_CHECK(mgr.is_alive(first.id()));
        mgr.kill(first.id());
        BOOST_CHECK(!mgr.is_alive(first.id()));

        auto second = mgr.create().id();

        // components
        BOOST_CHECK(!mgr.has_component<int>(second));
        mgr.add_component<int>(second, 1);
        BOOST_CHECK_EQUAL(mgr.get_component<int>(second), 1);
        /*BOOST_CHECK_EQUAL(h.has<float>(), false);
        mgr.mHasInt[1] = true;
        BOOST_CHECK_EQUAL(h.has<int>(), true);


        h.add<float>(5.f);
        BOOST_CHECK_EQUAL(h.has<float>(), true);
        BOOST_CHECK_EQUAL(h.get<float>(), 5.f);

        h.get<int>() = 2;
        BOOST_CHECK_EQUAL(h.get<int>(), 2);*/
    }

BOOST_AUTO_TEST_SUITE_END()
