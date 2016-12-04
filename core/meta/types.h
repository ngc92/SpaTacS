//
// Created by erik on 12/3/16.
//

#ifndef SPATACS_CORE_TYPES_H
#define SPATACS_CORE_TYPES_H

#include <cstddef>
#include <type_traits>

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
            constexpr type_t() = default;
            using type = T;
        };

        //! dumb wrapper for a parameter pack.
        template<class... T>
        struct pack_t
        {
            constexpr pack_t() = default;
            constexpr static const std::size_t size = sizeof...(T);
        };

        //! compile time boolean
        template<bool b>
        struct bool_t
        {
            constexpr static const bool value = b;
            constexpr explicit operator bool() const { return value; }
        };

        // -------------------------------------------------------------------------------------------------------------
        // variable templates for convenience
        template<class T>
        constexpr type_t<T> type_v;

        template<class... T>
        constexpr pack_t<T...> pack_v;

        // -------------------------------------------------------------------------------------------------------------
        // operations on types
        template<class T, class U>
        constexpr bool_t<std::is_same<T, U>::value> operator==(type_t<T>, type_t<U>) { return {}; };

        template<class T, class U>
        constexpr bool_t<!std::is_same<T, U>::value> operator!=(type_t<T>, type_t<U>) { return {}; };


        // -------------------------------------------------------------------------------------------------------------
        // transformations
        template<class T>
        constexpr type_t<std::decay_t<T>> decay(type_t<T> t) { return {}; }

        template<class... T>
        constexpr pack_t<std::decay_t<T>...> decay(pack_t<T...> t) { return {}; }

        template<template<class...> class T, class... Args>
        constexpr pack_t<Args...> args_of(type_t<T<Args...>> t) { return {}; };

        // -------------------------------------------------------------------------------------------------------------
    }
    template<class T>
    using type_t = meta::type_t<T>;

    template<class... T>
    using pack_t = meta::pack_t<T...>;

    template<bool b>
    using bool_t = meta::bool_t<b>;

    // -----------------------------------------------------------------------------------------------------------------
    // variable templates for convenience
    template<class T>
    constexpr type_t<T> type_v;

    template<class... T>
    constexpr pack_t<T...> pack_v;
}
}

#endif //SPATACS_CORE_TYPES_H
