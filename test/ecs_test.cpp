//
// Created by erik on 11/19/16.
//


#include "physics/PhysicsWorld.h"
#include <boost/test/unit_test.hpp>
#include <iostream>

#include "core/V2/EntityManager.h"
#include "core/V2/ComponentStorage.h"
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

        BOOST_CHECK_EQUAL( cs.getNextFreeIndex(), 0 );

        // Lifetime mgm
        for(int i = 0; i < 10; ++i) {
            BOOST_CHECK_EQUAL(cs.is_alive(i), false);
        }

        cs.create(2);
        BOOST_CHECK_EQUAL(cs.is_alive(2), true);

        cs.kill(2);
        BOOST_CHECK_EQUAL(cs.is_alive(2), false);
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
        T& get_component(id_t id)
        {
            if(std::is_same<T, int>::value) return reinterpret_cast<T&>(mIntValues.at(id));
            return  reinterpret_cast<T&>(mFloatValues.at(id));
        }

        std::vector<bool> mAlive;
        std::vector<bool> mHasInt;
        std::vector<bool> mHasFloat;

        std::vector<int> mIntValues;
        std::vector<float> mFloatValues;
    };

    BOOST_AUTO_TEST_CASE(EntityHandle)
    {
        struct MockConfig
        {
            using manager_t = MockMgr;
            using id_t      = std::size_t;
        };

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

BOOST_AUTO_TEST_SUITE_END()