//
// Created by erik on 12/3/16.
//

#ifndef SPATACS_CORE_TYPES_H
#define SPATACS_CORE_TYPES_H

#include <cstddef>

namespace spatacs
{
namespace core
{
namespace meta
{
    //! dumb type wrapper, usefull for all kinds of metaprogramming
    template<class T>
    struct type_t
    {
        using type = T;
    };

    //! dumb wrapper for a parameter pack.
    template<class... T>
    struct pack_t
    {
        constexpr static const std::size_t size = sizeof...(T);
    };
}
    template<class T>
    using type_t = meta::type_t<T>;

    template<class... T>
    using pack_t = meta::pack_t<T...>;
}
}

#endif //SPATACS_CORE_TYPES_H
