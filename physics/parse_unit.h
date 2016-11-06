//
// Created by erik on 10/21/16.
//

#ifndef SPATACS_PARSE_UNIT_H
#define SPATACS_PARSE_UNIT_H

#include <string>
#include <ratio>
#include <cmath>

namespace spatacs
{
namespace physics
{
    namespace parse_unit {
        struct rtratio
        {
            int num = 0;
            int den = 1;
        };

        template<std::intmax_t N, std::intmax_t D>
        bool operator==(rtratio rr, std::ratio<N, D> cr)
        {
            return rr.num * cr.den == cr.num * rr.den;
        };

        struct rtdim
        {
            rtratio meters{};
            rtratio kgrams{};
            rtratio seconds{};
        };

        struct rtunit
        {
            bool valid;
            rtdim dim;
            double factor;
        };

        rtunit parse(std::string unit_s);
    }
}
}

#endif //SPATACS_PARSE_UNIT_H
