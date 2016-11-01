//
// Created by erik on 10/28/16.
//

#include "physics/units.h"
#include <boost/test/unit_test.hpp>
#include <type_traits>
#include <sstream>

using namespace spatacs;
namespace dim = physics::dimensions;

// check dimension multiplication
static_assert( std::is_same<dim::mul_t<dim::dimless_t, dim::length_t>, dim::length_t>::value,  "multiplication of dimensionless and length must yield length");
static_assert( std::is_same<dim::mul_t<dim::dimless_t, dim::energy_t>, dim::energy_t>::value,  "multiplication of dimensionless and energy must yield energy");


static_assert( std::is_same<dim::mul_t<dim::acceleration_t, dim::mass_t>, dim::force_t>::value,  "error in unit multiplication");
static_assert( std::is_same<dim::mul_t<dim::force_t, dim::length_t>, dim::energy_t>::value,  "error in unit multiplication");
static_assert( std::is_same<dim::div_t<dim::length_t, dim::time_t>, dim::velocity_t>::value,  "error in unit division");
static_assert( std::is_same<dim::div_t<dim::velocity_t, dim::time_t>, dim::acceleration_t>::value,  "error in unit division");
static_assert( std::is_same<dim::pow_t<dim::area_t, 1, 2>, dim::length_t>::value,  "error in unit division");

static_assert( std::ratio_equal<dim::energy_t::length, std::ratio<2, 1>>(), "unexpected meters in energy");
static_assert( std::ratio_equal<dim::energy_t::time,   std::ratio<-2, 1>>(), "unexpected meters in energy");
static_assert( std::ratio_equal<dim::energy_t::mass, std::ratio<1, 1>>(), "unexpected meters in energy");

static_assert( dim::dimensions_equal<dim::acceleration_t, dim::acceleration_t>(), "equality comparison is broken" );

#define STREAM_TEST(value, result)  \
{                                   \
    std::stringstream stream;       \
    stream << value;                \
    BOOST_CHECK_EQUAL(stream.str(), result); \
}

BOOST_AUTO_TEST_SUITE(Units)
    BOOST_AUTO_TEST_CASE(ratio_stream)
    {
        STREAM_TEST((std::ratio<1, 3>{}), "1/3");
        STREAM_TEST((std::ratio<3, 1>{}), "3");
    }

    BOOST_AUTO_TEST_CASE(dimension_stream)
    {
        STREAM_TEST(dim::length_t{}, "m");
        STREAM_TEST(dim::area_t{},   "m^2");
        STREAM_TEST(dim::energy_t{}, "kgm^2s^-2");
    }

    BOOST_AUTO_TEST_CASE(units_stream)
    {
        STREAM_TEST(500.0_m, "500m");
        STREAM_TEST(150.0_kg, "150kg");
        STREAM_TEST(500.0_kJ, "500kJ");
        STREAM_TEST(3000.0_kg, "3t");
        STREAM_TEST(kilonewtons(5.5), "5.5kN");
        STREAM_TEST(5.0_km*5.0_km, "5km^2");
    }
BOOST_AUTO_TEST_SUITE_END()