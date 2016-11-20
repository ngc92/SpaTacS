//
// Created by erik on 11/20/16.
//

#ifndef SPATACS_CORE_MP_H
#define SPATACS_CORE_MP_H

#include <utility>
#include <type_traits>

namespace spatacs
{
namespace core
{
namespace ecs
{
    //! dumb type wrapper, usefull for all kinds of metaprogramming
    template<class T>
    struct type_t
    {
        using type = T;
    };
}
}
}

#endif //SPATACS_CORE_MP_H
