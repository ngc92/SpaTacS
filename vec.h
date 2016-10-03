//
// Created by erik on 9/20/16.
//

#ifndef SOI_VEC_H
#define SOI_VEC_H

#include <iosfwd>
#include <ostream>
#include <cmath>
#include <tuple>

namespace spatacs
{
    namespace physics {
        template<class T>
        struct Vec3
        {
            Vec3() = default;

            template<class A, class B, class C>
            Vec3(A x_, B y_, C z_) : x(x_), y(y_), z(z_)
            {}

            template<class S>
            explicit Vec3(const Vec3<S>& o) : x(T(o.x)), y(T(o.y)), z(T(o.z))
            {}

            T x;
            T y;
            T z;
        };

        // operators
        template<class T, class S>
        auto operator+(const Vec3<T>& a, const Vec3<S>& b)
        {
            return Vec3<decltype(a.x + b.x)>{a.x + b.x, a.y + b.y, a.z + b.z};
        }

        template<class T, class S>
        auto operator-(const Vec3<T>& a, const Vec3<S>& b)
        {
            return Vec3<decltype(a.x - b.x)>{a.x - b.x, a.y - b.y, a.z - b.z};
        }

        template<class T>
        auto operator*(const Vec3<T>& a, double s)
        {
            return Vec3<T>{a.x * s, a.y * s, a.z * s};
        }

        template<class S, class T>
        auto operator*(const S& s, const Vec3<T>& a)
        {
            return a * s;
        };

        template<class T, class S>
        Vec3<T>& operator+=(Vec3<T>& a, const Vec3<S>& b)
        {
            a.x += b.x;
            a.y += b.y;
            a.z += b.z;
            return a;
        };

        template<class T, class S>
        Vec3<T>& operator-=(Vec3<T>& a, const Vec3<S>& b)
        {
            a.x -= b.x;
            a.y -= b.y;
            a.z -= b.z;
            return a;
        };

        template<class T, class S>
        Vec3<T>& operator*=(Vec3<T>& a, const S& s)
        {
            a.x *= s;
            a.y *= s;
            a.z *= s;
            return a;
        };

        template<class T>
        bool operator==(const Vec3<T>& a, const Vec3<T>& b)
        {
            return std::tie(a.x, a.y, a.z) == std::tie(b.x, b.y, b.z);
        }

        // utility functions
        template<class T, class S>
        auto dot(const Vec3<T>& a, const Vec3<S>& b)
        {
            return a.x * b.x + a.y * b.y + a.z * b.z;
        };

        template<class T>
        T length(const Vec3<T>& vec)
        {
            using std::sqrt;
            return sqrt(dot(vec, vec));
        }

        template<class T, class S>
        auto parallel(const Vec3<T>& source, const Vec3<S>& reference)
        {
            auto s = dot(reference, reference);
            auto f = dot(source, reference);

            return (f / s) * reference;
        }

        template<class T, class S>
        auto perpendicular(const Vec3<T>& source, const Vec3<S>& reference)
        {
            return source - parallel(source, reference);
        }


        template<class T>
        std::ostream& operator<<(std::ostream& os, const Vec3<T>& vec)
        {
            return os << "(" << vec.x << ", " << vec.y << ", " << vec.z << ")";
        }

        template<class T>
        std::istream& operator>>(std::istream& is, Vec3<T>& vec)
        {
            return is >> vec.x >> vec.y >> vec.z;
        }
    }

    using Vec = physics::Vec3<float>;
}

#endif //SOI_VEC_H