//
// Created by erik on 9/23/16.
//

#include <cmath>
#include <boost/optional.hpp>
#include "HitTests.h"

using namespace spatacs;

boost::optional<physics::time_t> physics::intersect_origin(const MovingSphere& s)
{
    // x²(t) = r²(t): v² t² + 2 vo t + o² = a²t² + 2 ar t + r²
    // (v² - a²) t² + 2 (vo - ar)t + o² - r² = 0
    // t² + 2 (vo - ar)/(v²-a²) t + (o²-r²)/(v²-a²) = 0
    auto vo = dot(s.vel, s.pos);
    auto ar = s.exp * s.rad;
    auto v2 = dot(s.vel, s.vel);
    auto a2 = s.exp * s.exp;
    auto o2 = dot(s.pos, s.pos);
    auto r2 = s.rad * s.rad;

    // extreme case, need to consider to prevent div by zero
    if(v2 == a2)
    {
        if(vo == ar)
            return boost::none;

        return (r2 - o2) / (vo - ar);
    }

    auto p = 2.0 * (vo - ar) / (v2 - a2);
    auto q = (o2 - r2) / (v2 - a2);
    auto det = p*p/4.0 - q;
    if(det < decltype(det)(0))
    {
        return boost::none;
    }
    auto w = sqrt(det);
    if(-p/2.0 < w)
    {
        return -p/2.0 + w;
    } else
    {
        return -p/2.0 - w;
    }
}

boost::optional<physics::time_t> physics::intersect(MovingSphere s1, const MovingSphere& s2)
{
    s1.pos -= s2.pos;
    s1.vel -= s2.vel;
    s1.rad += s2.rad;
    s1.exp += s2.exp;
    return intersect_origin(s1);
}
