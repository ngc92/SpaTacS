//
// Created by erik on 12/3/16.
//

#ifndef SPATACS_CORE_FIND_H
#define SPATACS_CORE_FIND_H

#include "types.h"

namespace spatacs
{
namespace core
{
namespace meta
{
    namespace detail
    {
        // resursively look for Needle in Haystack, and build up the index that results from it.
        // if not found, an index size(Haystack) + 1 is returned.

        template<class Needle>
        constexpr std::size_t find_imp(type_t<Needle> n, pack_t<>)
        {
            return 0;
        };

        template<class Needle, class First, class... Haystack>
        constexpr std::size_t find_imp(type_t<Needle> n, pack_t<First, Haystack...>)
        {
            if(std::is_same<Needle, First>::value)
            {
                return 0;
            } else
            {
                return 1 + find_imp(n, pack_v<Haystack...> );
            }
        };

        static_assert(find_imp(type_v<int>,    pack_v<int, float>) == 0, "Error in find!");
        static_assert(find_imp(type_v<float>,  pack_v<int, float>) == 1, "Error in find!");
        static_assert(find_imp(type_v<double>, pack_v<int, float>) == 2, "Error in find!");
    }

    template<class Needle, class... Haystack>
    constexpr std::size_t find(type_t<Needle> n, pack_t<Haystack...> h)
    {
        constexpr std::size_t candidate = detail::find_imp(n, h);
        static_assert(candidate < h.size, "Could not find Needle in Haystack");
        return candidate;
    };

    template<class Needle, class... Haystack>
    constexpr auto has(type_t<Needle> n, pack_t<Haystack...> h)
    {
        constexpr std::size_t index = detail::find_imp(n, h);
        return bool_t<index < h.size>{};
    };
}
}
}

#endif //SPATACS_CORE_FIND_H
