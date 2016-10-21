//
// Created by erik on 10/21/16.
//

#include <regex>
#include <iostream>

#include "parse_unit.h"

using namespace spatacs::physics::parse_unit;

rtratio operator+(rtratio a, rtratio b)
{
    auto den = a.den * b.den;
    auto num = a.den * b.num + b.den * a.num;
    return rtratio{num, den};
}

rtratio operator*(rtratio a, rtratio b)
{
    return rtratio{a.num * b.num, a.den * b.den};
}

std::ostream& operator<<(std::ostream& s, const rtratio& r)
{
    s << r.num;
    if(r.den != 1)
        s << "/" << r.den;
    return s;
}

rtdim& operator+=(rtdim& a, const rtdim& b)
{
    a.meters = a.meters + b.meters;
    a.kgrams = a.kgrams+ b.kgrams;
    a.seconds = a.seconds + b.seconds;
    return a;
}

rtdim& operator*=(rtdim& a, const rtratio& b)
{
    a.meters = a.meters * b;
    a.kgrams = a.kgrams * b;
    a.seconds = a.seconds * b;
    return a;
}

namespace {
    rtunit parse_single_factor(std::string& unit)
    {
        std::smatch m;
        std::regex e(R"(([kMG]?)([gmstJ])(\^(-?[[:d:]]+)(\/([[:d:]]+))?)?)");
        if (std::regex_search(unit, m, e, std::regex_constants::match_continuous)) {
            double factor = 1;
            rtdim dims{};
            rtratio exponent{1, 1};

            enum State
            {
                BEGIN,
                EXP,
                DEN
            } state = BEGIN;

            bool first = true;
            for (const auto& x:m) {
                // ignore first match, this is the whole sequence. IF that is just one character,
                // we could misinterpret it.
                if (first) {
                    first = false;
                    continue;
                }
                if (x == "k") { factor = 1000; }
                if (x == "M") { factor = 1000000; }
                if (x == "G") { factor = 1000000000; }

                if (x == "g") {
                    dims.kgrams.num = 1;
                    factor *= 0.001;
                }
                if (x == "m") { dims.meters.num = 1; }
                if (x == "s") { dims.seconds.num = 1; }
                if (x == "t") {
                    dims.kgrams.num = 1;
                    factor *= 1000;
                }
                if (x == "J") {
                    dims.kgrams.num = 1;
                    dims.meters.num = 2;
                    dims.seconds.num = -2;
                }

                if (state == EXP) { exponent.num = std::stoi(x); }
                if (state == DEN) { exponent.den = std::stoi(x); }

                if (x.str()[0] == '^') { state = EXP; }
                if (x.str()[0] == '/') { state = DEN; }
            }

            dims *= exponent;
            unit = m.suffix();

            return rtunit{true, dims, std::pow(factor, double(exponent.num) / exponent.den)};
        }

        return rtunit{false, rtdim{}, 0.0};
    }

    rtunit parse_imp(std::string unit_s)
    {
        std::string copy = unit_s;
        rtunit u{};
        u.factor = 1.0;
        rtunit parsed = parse_single_factor(unit_s);
        int mode = +1;
        while (parsed.valid) {
            if (mode == -1) {
                parsed.factor = 1.0 / parsed.factor;
                parsed.dim *= rtratio{-1, 1};
            }
            u.factor *= parsed.factor;
            u.dim    += parsed.dim;
            if (unit_s[0] == '/') {
                mode = -1;
                unit_s = unit_s.substr(1);
            }
            parsed = parse_single_factor(unit_s);
        }

        u.valid = unit_s == "";
        return u;
    }
}

namespace spatacs
{
    namespace physics
    {
        namespace parse_unit
        {
            rtunit parse(std::string unit_str)
            {
                return parse_imp(std::move(unit_str));
            }
        }
    }
}