//
// Created by erik on 10/3/16.
//

#include "physics/HitTests.h"
#include <boost/test/unit_test.hpp>
#include <boost/optional.hpp>
#include <boost/optional/optional_io.hpp>
#include <iostream>


BOOST_AUTO_TEST_SUITE(Collisions)

    BOOST_AUTO_TEST_CASE(head_on)
    {
        using namespace spatacs::physics;
        // stationary reference target
        MovingSphere sp_stat{ length_vec{0, 0, 0}, velocity_vec{0,0,0}, length_t(1), speed_t{0} };
        MovingSphere sp_mov1{ length_vec{0, 0, 10}, velocity_vec{0,0,-1}, length_t(1), speed_t{0} };

        auto res = intersect(sp_stat, sp_mov1);
        BOOST_CHECK( (bool)res );
        if(res)
        {
            auto t = res.value();
            BOOST_CHECK_EQUAL( t, 8.0_s );
        }

        sp_mov1.vel = velocity_vec{0,0,-2};
        BOOST_CHECK_EQUAL( intersect(sp_stat, sp_mov1), 4.0_s );
        sp_stat.vel = velocity_vec{0,0,-1};
        BOOST_CHECK_EQUAL( intersect(sp_stat, sp_mov1), 8.0_s );
    }

    BOOST_AUTO_TEST_CASE(parallel)
    {
        using namespace spatacs::physics;
        // stationary reference target
        MovingSphere sp_stat{ length_vec{0, 0.5, 0}, velocity_vec{0,0,0}, length_t(1), speed_t{0} };
        MovingSphere sp_mov1{ length_vec{0, 0, 10}, velocity_vec{0,0,-1}, length_t(2), speed_t{0} };

        auto res = intersect(sp_stat, sp_mov1);
        BOOST_CHECK( (bool)res );
        BOOST_CHECK_CLOSE( length(sp_mov1.pos + res.value()*sp_mov1.vel - sp_stat.pos).value, (sp_stat.rad + sp_mov1.rad).value, 1e-6f );

        sp_stat.pos.y = 1.0_km;
        res = intersect(sp_stat, sp_mov1);
        BOOST_CHECK( (bool)res );
        BOOST_CHECK_CLOSE( length(sp_mov1.pos + res.value()*sp_mov1.vel - sp_stat.pos).value, (sp_stat.rad + sp_mov1.rad).value, 1e-6f );

        sp_stat.pos.y = 3.51_km;
        res = intersect(sp_stat, sp_mov1);
        std::cout << res << "\n";
        BOOST_CHECK( !(bool)res );
    }

    BOOST_AUTO_TEST_CASE(within)
    {
        using namespace spatacs::physics;
        // stationary reference target
        MovingSphere sp_stat{ length_vec{0, 0, 0}, velocity_vec{0,0,0}, length_t(3), speed_t{0} };
        MovingSphere sp_mov1{ length_vec{0, 0, 1}, velocity_vec{0,0,1}, length_t(1), speed_t{0} };

        auto res = intersect(sp_stat, sp_mov1);
        BOOST_CHECK( (bool)res );
        BOOST_CHECK_CLOSE( res.get().value, 1.0, 1e-6 );
    }

BOOST_AUTO_TEST_SUITE_END()
