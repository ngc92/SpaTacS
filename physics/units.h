//
// Created by erik on 10/1/16.
//

#ifndef SOI_UNITS_H
#define SOI_UNITS_H

#include "vec.h"
#include "dimensions.h"
#include <iosfwd>
#include <iostream>
#include "parse_unit.h"

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
    constexpr UnitWrapper<T, U> operator*(const T& f, const UnitWrapper<T, U>& a)
    {
        auto c = a;
        c *= f;
        return c;
    };

    // unit multiplication
    template<class T, class U, class V>
    UnitWrapper<T, dimensions::mul_t<U, V>>
    operator*( const UnitWrapper<T, U>& a, const UnitWrapper<T, V>& b)
    {
        return UnitWrapper<T, dimensions::mul_t<U, V>> (a.value * b.value);
    };

    template<class T, class U, class V>
    constexpr UnitWrapper<T, U>& operator*=(UnitWrapper<T, U>& a, const UnitWrapper<T, V>& b)
    {
        a = a*b;
        return a;
    };

    template<class T, class U, class V>
    constexpr UnitWrapper<T, dimensions::div_t<U, V>>
    operator/( const UnitWrapper<T, U>& a, const UnitWrapper<T, V>& b)
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

    template<class T, class U>
    constexpr bool operator!=(const UnitWrapper<T, U>& a, const UnitWrapper<T, U>& b)
    {
        return !(a==b);
    };

    // vector multiplication
    template<class T, class U, class V>
    constexpr auto operator*( const Vec3<T>& a, const UnitWrapper<U, V>& s ) -> Vec3<decltype(a.x*s)>
    {
        return Vec3<decltype(a.x*s)>{a.x*s, a.y*s, a.z*s};
    }

    // vector division
    template<class T, class U, class V>
    constexpr auto operator/( const Vec3<T>& a, const UnitWrapper<U, V>& s ) -> Vec3<decltype(a.x/s)>
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

    /// this function safely calculated the ratio of \p a and \p b, taking into account
    /// b == 0.
    /// \return a/b, or 1 if b == 0.
    template<class U, class V>
    U ratio( const UnitWrapper<U, V>& a, const UnitWrapper<U, V>& b )
    {
        if( b == UnitWrapper<U, V>{U(0)})
            return U(1);
        return a / b;
    };

    template<class O, class T, class U>
    std::basic_ostream<O>& operator<<(std::basic_ostream<O>& stream, const UnitWrapper<T, U>& u)
    {
        if(u.value < 2000)
            return stream << u.value << U{};
        else if(u.value < 2000000)
        return stream << u.value / 1000. << O('k') << U{};
        else
            return stream << u.value / 1000000. << O('M') << U{};
    }

    // specialisations
    template<class O, class T>
    std::basic_ostream<O>& operator<<(std::basic_ostream<O>& stream, const UnitWrapper<T, dimensions::mass_t>& u)
    {
        if(u.value < 2000)
            return stream << u.value << "kg";
        else if(u.value < 2000000)
            return stream << u.value / 1000. << O('t');
        else
            return stream << u.value / 1000000. << "kt";
    }

    template<class O, class T>
    std::basic_ostream<O>& operator<<(std::basic_ostream<O>& stream, const UnitWrapper<T, dimensions::force_t>& u)
    {
        if(u.value < 2000)
            return stream << u.value << "N";
        else if(u.value < 2000000)
            return stream << u.value / 1000. << "kN";
        else
            return stream << u.value / 1000000. << "MN";
    }

    template<class O, class T>
    std::basic_ostream<O>& operator<<(std::basic_ostream<O>& stream, const UnitWrapper<T, dimensions::energy_t>& u)
    {
        if(u.value < 2000)
            return stream << u.value << "J";
        else if(u.value < 2000000)
            return stream << u.value / 1000. << "kJ";
        else
            return stream << u.value / 1000000. << "MJ";
    }

    template<class I, class T, class U>
    std::basic_istream<I>& operator>>(std::basic_istream<I>& stream, UnitWrapper<T, U>& u)
    {
        T val;
        std::string unit;

        stream >> val;
        stream >> unit;

        auto result = parse_unit::parse(unit);
        if(result.valid &&
           result.dim.meters  == typename U::length{} &&
           result.dim.kgrams  == typename U::mass{} &&
           result.dim.seconds == typename U::time{})
        {
            val *= result.factor;
        } else
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
        using impulse_t = UnitWrapper<base_t, dimensions::impulse_t>;
        using accel_t   = UnitWrapper<base_t, dimensions::acceleration_t>;
        using force_t   = UnitWrapper<base_t, dimensions::force_t>;
        using area_t    = UnitWrapper<base_t, dimensions::area_t>;
        using energy_t  = UnitWrapper<base_t, dimensions::energy_t>;
        using power_t   = UnitWrapper<base_t, dimensions::power_t>;

        template<class T>
        using rate_t    = UnitWrapper<base_t, dimensions::rate_dim_t<typename T::dimension_t>>;

        template<class T>
        using inverse_t = UnitWrapper<base_t, dimensions::inverse_dim_t<typename T::dimension_t>>;

        using length_vec   = Vec3<length_t>;
        using velocity_vec = Vec3<speed_t>;
        using accel_vec    = Vec3<accel_t>;
        using force_vec    = Vec3<force_t>;
        using impulse_vec  = Vec3<impulse_t>;

        // creation functions
        inline constexpr auto meters(base_t v) { return length_t(v); }
        inline constexpr auto kilometers(base_t v) { return length_t(v * base_t(1000)); }

        constexpr auto meters(Vec v) { return length_vec(v); };
        constexpr auto kilometers(Vec v) { return length_vec(v)*base_t(1000); };
        constexpr auto meters(base_t x, base_t y, base_t z) { return meters(Vec{x, y, z}); }
        constexpr auto kilometers(base_t x, base_t y, base_t z) { return kilometers(Vec{x, y, z}); }

        constexpr auto kilogram(base_t v) { return mass_t(v); }
        constexpr auto tonnes(base_t v) { return mass_t(v * 1000); }

        constexpr auto newtons(base_t v) { return force_t(v); }
        constexpr auto kilonewtons(base_t v) { return force_t(v * 1000); }

        constexpr auto joules(base_t v) { return energy_t(v); }
        constexpr auto kilojoules(base_t v) { return energy_t(v * 1000); }
        constexpr auto megajoules(base_t v) { return energy_t(v * 1000 * 1000); }

        constexpr auto watts(base_t v) { return power_t(v); }
        constexpr auto kilowatts(base_t v) { return power_t(v * 1000); }
        constexpr auto megawatts(base_t v) { return power_t(v * 1000 * 1000); }

        inline constexpr length_t operator ""_m(long double f) { return meters(f); }
        inline constexpr length_t operator ""_km(long double f) { return kilometers(f); }

        inline constexpr mass_t operator ""_kg(long double f) { return kilogram(f); }
        inline constexpr mass_t operator ""_t(long double f) { return tonnes(f); }

        inline constexpr energy_t operator ""_J(long double f) { return joules(f); }
        inline constexpr energy_t operator ""_kJ(long double f) { return kilojoules(f); }
        inline constexpr energy_t operator ""_MJ(long double f) { return megajoules(f); }

        inline constexpr power_t operator ""_W(long double f) { return watts(f); }
        inline constexpr power_t operator ""_kW(long double f) { return kilowatts(f); }
        inline constexpr power_t operator ""_MW(long double f) { return megawatts(f); }

        inline constexpr time_t operator ""_s(long double f) { return time_t(f); }
        inline constexpr speed_t operator ""_kps(long double f) { return kilometers(f) / 1.0_s; }
    }

    using namespace unit_types;
}
    using namespace physics::unit_types;
}

#endif //SOI_UNITS_H
