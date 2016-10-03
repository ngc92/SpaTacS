//
// Created by erik on 10/1/16.
//

#include "vec.h"
#define BOOST_TEST_MODULE VEC
#include <boost/test/included/unit_test.hpp>


BOOST_AUTO_TEST_SUITE(VecTest)
    using spatacs::Vec;
    BOOST_AUTO_TEST_CASE(OPS)
    {
        Vec a = {1, 2, 3};
        Vec b = {5, 4, 7};

        BOOST_CHECK_EQUAL(a + b, (Vec{6, 6, 10}));
        Vec c = a;
        c += b;
        BOOST_CHECK_EQUAL( c , a + b );

        BOOST_CHECK_EQUAL( b - a , (Vec{4, 2, 4}) );
        c -= b;
        BOOST_CHECK_EQUAL(c , a);

        float f = 2;
        BOOST_CHECK_EQUAL(1*a , a);
        BOOST_CHECK_EQUAL(f*a , a*f);
        BOOST_CHECK_EQUAL(2*a , (Vec{2, 4, 6}));
        c *= f;
        BOOST_CHECK_EQUAL(c , f*a);
    }

    BOOST_AUTO_TEST_CASE(Length)
    {
        Vec a = {1, 0, 0};
        BOOST_CHECK_EQUAL(1 , length(a));

        Vec b = {2, 2, 1};
        BOOST_CHECK_EQUAL(3 , length(b));

        BOOST_CHECK_EQUAL(length(2*b) , 2*length(b));
    }

    BOOST_AUTO_TEST_CASE(Dot)
    {
        Vec a = {1, 3, 2};
        Vec b = {2, 2, 1};
        Vec c = {0, 3, 1};
        BOOST_CHECK_EQUAL(dot(b, b) , length(b) * length(b));

        Vec ex = {1, 0, 0};
        Vec ey = {0, 1, 0};
        Vec ez = {0, 0, 1};
        BOOST_CHECK_EQUAL(dot(ex, b) , b.x);
        BOOST_CHECK_EQUAL(dot(ey, b) , b.y);
        BOOST_CHECK_EQUAL(dot(ez, b) , b.z);

        BOOST_CHECK_EQUAL( dot(a+b, c) , dot(a, c) + dot(b, c));
        BOOST_CHECK_EQUAL( dot(a, b) , dot(b, a));
        BOOST_CHECK_EQUAL( dot(2*a, b) , 2*dot(a, b));
    }

    BOOST_AUTO_TEST_CASE(Decomposition)
    {
        Vec a = {1, 3, 2};
        Vec b = {2, 2, 1};
        Vec ex = {1, 0, 0};

        BOOST_CHECK_EQUAL(parallel(a, ex) , (Vec{1, 0, 0}));
        BOOST_CHECK_EQUAL(perpendicular(a, ex) , (Vec{0, 3, 2}));

        BOOST_CHECK_EQUAL(parallel(a, b) + perpendicular(a, b) , a);
        BOOST_CHECK_EQUAL(parallel(a, b) , parallel(a, 2*b));
        BOOST_CHECK_EQUAL(perpendicular(a, b) , perpendicular(a, 2*b));

        BOOST_CHECK_SMALL( dot(parallel(a, b), perpendicular(a, b)), 1e-6f );
    }
BOOST_AUTO_TEST_SUITE_END()
