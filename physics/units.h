//
// Created by erik on 10/1/16.
//

#ifndef SOI_UNITS_H
#define SOI_UNITS_H

#include "vec.h"
#include "dimensions.h"
#include <iosfwd>

namespace spatacs
{
namespace physics
{
    template <class T, class U>
    class UnitWrapper
    {
    public:
        UnitWrapper() : value(T{}) { }
        explicit constexpr UnitWrapper( const T& val ) : value(val) { };
        explicit constexpr operator T() const { return value; }
        T value;
    };

    template <class T>
    class UnitWrapper<T, dimensions::dimless_t >
    {
    public:
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

    template<class O, class T, class U>
    std::basic_ostream<O>& operator<<(std::basic_ostream<O>& stream, const UnitWrapper<T, U>& u)
    {
        if(u.value < 2000)
            return stream << u.value << U{};
        else
            return stream << u.value / 1000. << O('k') << U{};
    }

    namespace unit_types {

        using base_t    = double;
        using length_t  = UnitWrapper<base_t, dimensions::length_t>;
        using mass_t    = UnitWrapper<base_t, dimensions::mass_t>;
        using time_t    = UnitWrapper<base_t, dimensions::time_t>;
        using speed_t   = UnitWrapper<base_t, dimensions::velocity_t>;
        using accel_t   = UnitWrapper<base_t, dimensions::acceleration_t>;
        using force_t   = UnitWrapper<base_t, dimensions::force_t>;
        using area_t    = UnitWrapper<base_t, dimensions::area_t>;

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

    }

    using namespace unit_types;

    inline constexpr length_t operator ""_km(long double f) { return kilometers(f); }
    inline constexpr length_t operator ""_m(long double f) { return meters(f); }

    inline constexpr mass_t operator ""_kg(long double f) { return kilogram(f); }
    inline constexpr mass_t operator ""_t(long double f) { return tonnes(f); }

    inline constexpr time_t operator ""_s(long double f) { return time_t(f); }
    inline constexpr speed_t operator ""_kps(long double f) { return kilometers(f) / 1.0_s; }
}
    using namespace physics::unit_types;

    using physics::operator""_km;
    using physics::operator""_m;
    using physics::operator""_kg;
    using physics::operator""_t;
    using physics::operator""_s;
    using physics::operator""_kps;
}

#endif //SOI_UNITS_H
