//
// Created by erik on 10/6/16.
//

#ifndef SPATACS_CONVERT_H
#define SPATACS_CONVERT_H

#include <irrlicht/vector3d.h>
#include "physics/units.h"

namespace spatacs
{
namespace ui
{
    irr::core::vector3df convert( const length_vec& l );
    length_vec convert( const irr::core::vector3df& l );

    irr::f32 convert( length_t l );
}
}

#endif //SPATACS_CONVERT_H
