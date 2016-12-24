//
// Created by erik on 11/19/16.
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

    using mock_id = spatacs::TaggedID<std::size_t, struct mock_t>;

    struct MockMgr;
    struct MockConfig
    {
        using manager_t = MockMgr;
        using id_t      = mock_id;
        using cmp_storage_t = ecs::ComponentStorage<int, float, double>;
        using comp_vec      = pack_t<int, float, double>;
        constexpr static std::size_t comp_count = 5;
    };

    struct MockMgr
    {
        using id_t = mock_id;

        MockMgr(std::size_t size) :
                mAlive(size, false),
                mHasInt(size, false),
                mHasFloat(size, false),
                mIntValues(size, 0),
                mFloatValues(size, 0.0)
        {

        };

        bool is_alive(id_t id) const { return mAlive[id.getID()]; }
        void kill(id_t id) { mAlive[id.getID()] = false; }

        template<class T>
        bool has_component(id_t id)
        {
            return std::is_same<T, int>::value ? mHasInt[id.getID()] : mHasFloat[id.getID()];
        }

        template<class T>
        T& add_component(id_t id, T&& t)
        {
            if(std::is_same<T, int>::value) {
                mHasInt.at(id.getID()) = true;
                return reinterpret_cast<T&>(mIntValues.at(id.getID()) = t);
            }
            mHasFloat.at(id.getID()) = true;
            return  reinterpret_cast<T&>(mFloatValues.at(id.getID()) = t);
        }

        template<class T>
        const T& get_component(id_t id) const
        {
            if(std::is_same<T, int>::value) return reinterpret_cast<T&>(mIntValues.at(id.getID()));
            return  reinterpret_cast<T&>(mFloatValues.at(id.getID()));
        }

        template<class T>
        T& get_mutable_component(id_t id)
        {
            if(std::is_same<T, int>::value) return reinterpret_cast<T&>(mIntValues.at(id.getID()));
            return  reinterpret_cast<T&>(mFloatValues.at(id.getID()));
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
        BOOST_CHECK_EQUAL(u.id(), mock_id{});

        MockMgr mgr(5);
        // initialized handle
        Handle h{mgr, MockConfig::id_t(1)};
        BOOST_CHECK_EQUAL(h.id(), mock_id{1});

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

        struct FakeConfig {
            using id_t = std::string;
        };

        auto double_f = [](double d) { BOOST_CHECK_EQUAL(d, 1.2); };
        data_t data {true, 2, 5.2f, 1.2};
        auto provider = [&](std::size_t){ return data; };

        auto double_fw = ecs::detail::make_forwarder(double_f, type_v<FakeConfig>, sig_t::types_t{}, provider);
        double_fw(0);
    }

    BOOST_AUTO_TEST_CASE(Signature_MutliParam)
    {
        using sig_t  = ecs::Signature<double, bool>;
        using data_t = std::tuple<bool, int, float, double>;

        struct FakeConfig {
            using id_t = std::string;
        };

        auto multi_f = [](double d, bool b) {
            BOOST_CHECK_EQUAL(d, 4.0);
            BOOST_CHECK(b);
        };
        data_t data {true, 2, 5.2f, 4.0};

        //! \todo I cannot capture by ref here, but I do not understand why.
        auto double_fw = ecs::detail::make_forwarder(multi_f, type_v<FakeConfig>, sig_t::types_t{}, [=](std::size_t){ return data; });
        BOOST_CHECK_NO_THROW(double_fw(0));
    }

    BOOST_AUTO_TEST_CASE(EntityManager)
    {
        using id_t = spatacs::TaggedID<std::size_t, struct ltg>;
        using Config = ecs::Config<id_t, int, double, float>;
        using EM_t   = ecs::EntityManager<Config>;

        EM_t mgr;

        // lifetime
        BOOST_CHECK(!mgr.is_alive(id_t{0}));
        BOOST_CHECK(!mgr.is_alive(id_t{1}));

        auto first = mgr.create();
        BOOST_CHECK_EQUAL(first.id(), id_t{1});
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
