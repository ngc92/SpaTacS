//
// Created by erik on 10/6/16.
//

#include "convert.h"

using namespace spatacs::physics;
using irr::core::vector3df;

const auto scale = 100.0_m;

vector3df spatacs::ui::convert(const length_vec& l)
{
    return irr::core::vector3df(l.x / scale, l.y / scale, l.z / scale);
}

length_vec spatacs::ui::convert(const vector3df& l)
{
    return length_vec((double)l.X * scale, (double)l.Y * scale, (double)l.Z * scale);
}

irr::f32 spatacs::ui::convert(length_t l)
{
    return l / scale;
}
