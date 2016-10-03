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
    UnitWrapper<T, U>& operator+=(UnitWrapper<T, U>& a, const UnitWrapper<T, U>& b)
    {
        a.value += b.value;
        return a;
    };

    template<class T, class U>
    UnitWrapper<T, U>& operator-=(UnitWrapper<T, U>& a, const UnitWrapper<T, U>& b)
    {
        a.value -= b.value;
        return a;
    };

    template<class T, class U>
    UnitWrapper<T, U> operator+(const UnitWrapper<T, U>& a, const UnitWrapper<T, U>& b)
    {
        auto c = a;
        c += b;
        return c;
    };

    template<class T, class U>
    UnitWrapper<T, U> operator-(const UnitWrapper<T, U>& a, const UnitWrapper<T, U>& b)
    {
        auto c = a;
        c -= b;
        return c;
    };

    template<class T, class U>
    UnitWrapper<T, U> operator-(const UnitWrapper<T, U>& a)
    {
        return UnitWrapper<T, U>{-a.value};
    };

    // --------------------------------------------------------------------------------
    //   scalar multiplication
    template<class T, class U>
    UnitWrapper<T, U>& operator*=(UnitWrapper<T, U>& a, const T& factor)
    {
        a.value *= factor;
        return a;
    };

    template<class T, class U>
    UnitWrapper<T, U> operator*(const UnitWrapper<T, U>& a, const T& f)
    {
        auto c = a;
        c *= f;
        return c;
    };

    template<class T, class U>
    UnitWrapper<T, U> operator/(const UnitWrapper<T, U>& a, const T& f)
    {
        return a * (T(1)/f);
    };

    template<class T, class U>
    auto operator*(const T& f, const UnitWrapper<T, U>& a)
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
    UnitWrapper<T, U>& operator*=(UnitWrapper<T, U>& a, const UnitWrapper<T, V>& b)
    {
        a = a*b;
        return a;
    };

    template<class T, class U, class V>
    auto operator/( const UnitWrapper<T, U>& a, const UnitWrapper<T, V>& b)
    {
        return UnitWrapper<T, dimensions::div_t<U, V>>(a.value / b.value);
    };

    // comparisons
    template<class T, class U>
    bool operator<(const UnitWrapper<T, U>& a, const UnitWrapper<T, U>& b)
    {
        return a.value < b.value;
    };

    template<class T, class U>
    bool operator<=(const UnitWrapper<T, U>& a, const UnitWrapper<T, U>& b)
    {
        return a.value <= b.value;
    }

    template<class T, class U>
    bool operator>=(const UnitWrapper<T, U>& a, const UnitWrapper<T, U>& b)
    {
        return a.value >= b.value;
    }

    template<class T, class U>
    bool operator>(const UnitWrapper<T, U>& a, const UnitWrapper<T, U>& b)
    {
        return a.value > b.value;
    };

    template<class T, class U>
    bool operator==(const UnitWrapper<T, U>& a, const UnitWrapper<T, U>& b)
    {
        return a.value == b.value;
    };

    // vector multiplication
    template<class T, class U, class V>
    auto operator*( const Vec3<T>& a, const UnitWrapper<U, V>& s )
    {
        return Vec3<decltype(a.x*s)>{a.x*s, a.y*s, a.z*s};
    }

    // vector division
    template<class T, class U, class V>
    auto operator/( const Vec3<T>& a, const UnitWrapper<U, V>& s )
    {
        return Vec3<decltype(a.x/s)>{a.x/s, a.y/s, a.z/s};
    }

    template<class U, class V>
    auto sqrt( const UnitWrapper<U, V>& s )
    {
        return UnitWrapper<U, dimensions::pow_t<V, 1, 2>>( std::sqrt(s.value) );
    };

    template<class T, class U>
    std::ostream& operator<<(std::ostream& stream, const UnitWrapper<T, U>& u)
    {
        return stream << u.value << U{}; //! \todo output units.
    }

    using base_t    = double;
    using length_t  = UnitWrapper<base_t, dimensions::length_t>;
    using mass_t    = UnitWrapper<base_t, dimensions::mass_t>;
    using time_t    = UnitWrapper<base_t, dimensions::time_t>;
    using speed_t   = UnitWrapper<base_t, dimensions::velocity_t>;
    using accel_t   = UnitWrapper<base_t, dimensions::acceleration_t>;
    using force_t   = UnitWrapper<base_t, dimensions::force_t>;

    using length_vec   = Vec3<length_t>;
    using velocity_vec = Vec3<speed_t>;
    using accel_vec    = Vec3<accel_t>;
    using force_vec    = Vec3<force_t>;

    inline constexpr length_t operator ""_km(long double f) { return length_t(f); }
    inline constexpr length_t operator ""_m(long double f) { return length_t(f/1000); }

    inline constexpr mass_t operator ""_kg(long double f) { return mass_t(f); }
    inline constexpr mass_t operator ""_t(long double f) { return mass_t(f*1000); }

    inline constexpr time_t operator ""_s(long double f) { return time_t(f); }
    inline constexpr speed_t operator ""_kps(long double f) { return speed_t(f); }
}
    using physics::length_t;
    using physics::mass_t;
    using physics::time_t;
    using physics::speed_t;
    using physics::accel_t;
    using physics::force_t;

    using physics::length_vec;
    using physics::velocity_vec;
    using physics::accel_vec;
    using physics::force_vec;

    using physics::operator""_km;
    using physics::operator""_m;
    using physics::operator""_kg;
    using physics::operator""_t;
    using physics::operator""_s;
    using physics::operator""_kps;
}

#endif //SOI_UNITS_H
