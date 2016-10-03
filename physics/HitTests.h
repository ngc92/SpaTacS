//
// Created by erik on 9/23/16.
//

#ifndef SOI_HITTESTS_H
#define SOI_HITTESTS_H

#include <boost/optional/optional_fwd.hpp>
#include "vec.h"
#include "units.h"

namespace spatacs
{
    namespace physics
    {
        struct MovingSphere
        {
            length_vec   pos;
            velocity_vec vel;
            length_t     rad;
            speed_t      exp;
            // x(t) = pos + vel * t
            // r(t) = rad + exp * t
        };

        boost::optional<time_t> intersect_origin(const MovingSphere& s);
        boost::optional<time_t> intersect(MovingSphere s1, const MovingSphere& s2);

    }
}

#endif //SOI_HITTESTS_H
