//
// Created by erik on 10/1/16.
//

#ifndef SOI_UNITS_H
#define SOI_UNITS_H

#include "vec.h"
#include "dimensions.h"
#include <iosfwd>
#include <iostream>

namespace spatacs
{
namespace physics
{
    template <class T, class U>
    class UnitWrapper
    {
    public:
        using dimension_t = U;

        UnitWrapper() : value(T{}) { }
        explicit constexpr UnitWrapper( const T& val ) : value(val) { };
        explicit constexpr operator T() const { return value; }
        T value;
    };

    template <class T>
    class UnitWrapper<T, dimensions::dimless_t >
    {
    public:
        using dimension_t = dimensions::dimless_t;
        UnitWrapper() : value(T{}) { }
        constexpr UnitWrapper( const T& val ) : value(val) { };
        constexpr operator T() const { return value; }
        T value;
    };

    // Abelian Group with respect to addition
    template<class T, class U>
    constexpr UnitWrapper<T, U>& operator+=(UnitWrapper<T, U>& a, const UnitWrapper<T, U>& b)
    {
        a.value += b.value;
        return a;
    };

    template<class T, class U>
    constexpr UnitWrapper<T, U>& operator-=(UnitWrapper<T, U>& a, const UnitWrapper<T, U>& b)
    {
        a.value -= b.value;
        return a;
    };

    template<class T, class U>
    constexpr UnitWrapper<T, U> operator+(const UnitWrapper<T, U>& a, const UnitWrapper<T, U>& b)
    {
        auto c = a;
        c += b;
        return c;
    };

    template<class T, class U>
    constexpr UnitWrapper<T, U> operator-(const UnitWrapper<T, U>& a, const UnitWrapper<T, U>& b)
    {
        auto c = a;
        c -= b;
        return c;
    };

    template<class T, class U>
    constexpr UnitWrapper<T, U> operator-(const UnitWrapper<T, U>& a)
    {
        return UnitWrapper<T, U>{-a.value};
    };

    // --------------------------------------------------------------------------------
    //   scalar multiplication
    template<class T, class U>
    constexpr UnitWrapper<T, U>& operator*=(UnitWrapper<T, U>& a, const T& factor)
    {
        a.value *= factor;
        return a;
    };

    template<class T, class U>
    constexpr UnitWrapper<T, U> operator*(const UnitWrapper<T, U>& a, const T& f)
    {
        auto c = a;
        c *= f;
        return c;
    };

    template<class T, class U>
    constexpr UnitWrapper<T, U> operator/(const UnitWrapper<T, U>& a, const T& f)
    {
        return a * (T(1)/f);
    };

    template<class T, class U>
    constexpr auto operator*(const T& f, const UnitWrapper<T, U>& a)
    {
        auto c = a;
        c *= f;
        return c;
    };

    // unit multiplication
    template<class T, class U, class V>
    auto operator*( const UnitWrapper<T, U>& a, const UnitWrapper<T, V>& b)
    {
        return UnitWrapper<T, dimensions::mul_t<U, V>>(a.value * b.value);
    };

    template<class T, class U, class V>
    constexpr UnitWrapper<T, U>& operator*=(UnitWrapper<T, U>& a, const UnitWrapper<T, V>& b)
    {
        a = a*b;
        return a;
    };

    template<class T, class U, class V>
    constexpr auto operator/( const UnitWrapper<T, U>& a, const UnitWrapper<T, V>& b)
    {
        return UnitWrapper<T, dimensions::div_t<U, V>>(a.value / b.value);
    };

    // comparisons
    template<class T, class U>
    constexpr bool operator<(const UnitWrapper<T, U>& a, const UnitWrapper<T, U>& b)
    {
        return a.value < b.value;
    };

    template<class T, class U>
    constexpr bool operator<=(const UnitWrapper<T, U>& a, const UnitWrapper<T, U>& b)
    {
        return a.value <= b.value;
    }

    template<class T, class U>
    constexpr bool operator>=(const UnitWrapper<T, U>& a, const UnitWrapper<T, U>& b)
    {
        return a.value >= b.value;
    }

    template<class T, class U>
    constexpr bool operator>(const UnitWrapper<T, U>& a, const UnitWrapper<T, U>& b)
    {
        return a.value > b.value;
    };

    template<class T, class U>
    constexpr bool operator==(const UnitWrapper<T, U>& a, const UnitWrapper<T, U>& b)
    {
        return a.value == b.value;
    };

    // vector multiplication
    template<class T, class U, class V>
    constexpr auto operator*( const Vec3<T>& a, const UnitWrapper<U, V>& s )
    {
        return Vec3<decltype(a.x*s)>{a.x*s, a.y*s, a.z*s};
    }

    // vector division
    template<class T, class U, class V>
    constexpr auto operator/( const Vec3<T>& a, const UnitWrapper<U, V>& s )
    {
        return Vec3<decltype(a.x/s)>{a.x/s, a.y/s, a.z/s};
    }

    template<class U, class V>
    auto sqrt( const UnitWrapper<U, V>& s )
    {
        return UnitWrapper<U, dimensions::pow_t<V, 1, 2>>( std::sqrt(s.value) );
    };

    template<class U, class V>
    auto abs( const UnitWrapper<U, V>& s )
    {
        return UnitWrapper<U, V>( std::abs(s.value) );
    };

    template<class O, class T, class U>
    std::basic_ostream<O>& operator<<(std::basic_ostream<O>& stream, const UnitWrapper<T, U>& u)
    {
        if(u.value < 2000)
            return stream << u.value << U{};
        else
            return stream << u.value / 1000. << O('k') << U{};
    }

    template<class I, class T, class U>
    std::basic_istream<I>& operator>>(std::basic_istream<I>& stream, UnitWrapper<T, U>& u)
    {
        T val;
        std::string unit;

        stream >> val;
        stream >> unit;

        // generate this at compile time!
        if( unit == "m" || unit == "km" )
        {
            if( !dimensions::dimensions_equal<U, dimensions::length_t>() )
            {
                stream.setstate( stream.failbit );
            }
            if(unit == "km")
                val *= 1000;
        }
         else if( unit == "mps" || unit == "m/s" || unit == "ms^-1")
        {
            if( !dimensions::dimensions_equal<U, dimensions::velocity_t>() )
            {
                stream.setstate( stream.failbit );
            }
        } else if( unit == "kps" || unit == "km/s" || unit == "kms^-1")
        {
            if( !dimensions::dimensions_equal<U, dimensions::velocity_t>() )
            {
                stream.setstate( stream.failbit );
            }
            val *= 1000;
        } else if( unit == "kg" || unit == "t" )
        {
            if( !dimensions::dimensions_equal<U, dimensions::mass_t>() )
            {
                stream.setstate( stream.failbit );
            }
            if(unit == "t")
                val *= 1000;
        }
        else if( unit == "kg/s" || unit == "t/s" )
        {
            if( !dimensions::dimensions_equal<U, dimensions::rate_dim_t<dimensions::mass_t>>() )
            {
                stream.setstate( stream.failbit );
            }
            if(unit == "t")
                val *= 1000;
        }
        else
        {
            stream.setstate( stream.failbit );
        }



        u.value = val;
        return stream;
    };

    namespace unit_types {

        using base_t    = double;

        using scalar_t  = UnitWrapper<base_t, dimensions::dimless_t>;
        using length_t  = UnitWrapper<base_t, dimensions::length_t>;
        using mass_t    = UnitWrapper<base_t, dimensions::mass_t>;
        using time_t    = UnitWrapper<base_t, dimensions::time_t>;
        using speed_t   = UnitWrapper<base_t, dimensions::velocity_t>;
        using accel_t   = UnitWrapper<base_t, dimensions::acceleration_t>;
        using force_t   = UnitWrapper<base_t, dimensions::force_t>;
        using area_t    = UnitWrapper<base_t, dimensions::area_t>;

        template<class T>
        using rate_t    = UnitWrapper<base_t, dimensions::rate_dim_t<typename T::dimension_t>>;

        using length_vec   = Vec3<length_t>;
        using velocity_vec = Vec3<speed_t>;
        using accel_vec    = Vec3<accel_t>;
        using force_vec    = Vec3<force_t>;

        // creation functions
        inline constexpr auto meters(base_t v) { return length_t(v); }
        inline constexpr auto kilometers(base_t v) { return length_t(v * base_t(1000)); }

        constexpr auto meters(Vec v) { return length_vec(v); };
        constexpr auto kilometers(Vec v) { return length_vec(v)*base_t(1000); };
        constexpr auto meters(base_t x, base_t y, base_t z) { return meters(Vec{x, y, z}); }
        constexpr auto kilometers(base_t x, base_t y, base_t z) { return kilometers(Vec{x, y, z}); }

        constexpr auto kilogram(base_t v) { return mass_t(v); }
        constexpr auto tonnes(base_t v) { return mass_t(v * 1000); }

        constexpr auto newton(base_t v) { return force_t(v); }
        constexpr auto kilonewton(base_t v) { return force_t(v * 1000); }

        inline constexpr length_t operator ""_km(long double f) { return kilometers(f); }
        inline constexpr length_t operator ""_m(long double f) { return meters(f); }

        inline constexpr mass_t operator ""_kg(long double f) { return kilogram(f); }
        inline constexpr mass_t operator ""_t(long double f) { return tonnes(f); }

        inline constexpr time_t operator ""_s(long double f) { return time_t(f); }
        inline constexpr speed_t operator ""_kps(long double f) { return kilometers(f) / 1.0_s; }
    }

    using namespace unit_types;
}
    using namespace physics::unit_types;
}

#endif //SOI_UNITS_H
