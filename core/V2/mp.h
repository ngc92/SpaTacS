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

    template<template<class...> class T>
    struct template_t
    {
    };

    template<class... T>
    struct type_vec_t
    {
    };

    /*
    template<template<class...> class T, class... S>
    auto substitute(template_t<T>, type_vec_t<S...>) -> type_t<T<S...>>
    {
        return {};
    }*/

    template<class Needle, class First, class... Haystack>
    constexpr std::size_t find(type_t<Needle> n, type_vec_t<First, Haystack...>)
    {
        if(std::is_same<Needle, First>::value)
        {
            return 0;
        } else
        {
            return 1 + find(n, type_vec_t<Haystack...>{} );
        }
    };

    template<class Needle>
    constexpr std::size_t find(type_t<Needle> n, type_vec_t<>)
    {
        /// \todo rewrite this to allow for static assertions here!
        return -1;
    };

    static_assert(find(type_t<int>{}, type_vec_t<int, float>{}) == 0, "Error in find!");
    static_assert(find(type_t<float>{}, type_vec_t<int, float>{}) == 1, "Error in find!");
}
}
}

#endif //SPATACS_CORE_MP_H
