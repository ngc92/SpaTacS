//
// Created by erik on 11/20/16.
//

#ifndef SPATACS_CORE_TRANSLATION_H
#define SPATACS_CORE_TRANSLATION_H

#include <cstddef>
#include <type_traits>
#include <tuple>

namespace spatacs
{
namespace core
{
namespace ecs
{
    /// \todo apply dionne paradigm here.
    namespace detail {
        template<bool Pred, class Needle, class... Haystack>
        class Helper;

        template<class Needle, class... Haystack>
        struct Helper<true, Needle, Haystack...>
        {
            static constexpr std::size_t rest = sizeof...(Haystack) + 1;
        };

        template<class Needle, class Next, class... Haystack>
        struct Helper<false, Needle, Next, Haystack...> :
                public Helper<std::is_same<Needle, Next>::value, Needle, Haystack...>
        {
        };

        template<class Needle>
        struct Helper<false, Needle>;

        template<class Needle, class... Haystack>
        constexpr std::size_t find_index()
        {
            return sizeof...(Haystack) - Helper<false, Needle, Haystack...>::rest;
        };

        template<class... Types>
        struct Translator
        {
            static constexpr std::size_t count = sizeof...(Types);

            template<class T>
            static constexpr std::size_t index() { return find_index<T, Types...>(); };
        };
    }

    template<class T>
    struct Translator;

    template<template<class...> typename C, class... V>
    struct Translator<C<V...>> : public detail::Translator<V...> { };
}
}
}

#endif //SPATACS_CORE_TRANSLATION_H
