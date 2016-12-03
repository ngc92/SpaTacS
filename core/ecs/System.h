//
// Created by erik on 11/20/16.
//

#ifndef SPATACS_CORE_SYSTEM_H
#define SPATACS_CORE_SYSTEM_H

#include <utility>
#include <tuple>
#include "mp.h"

namespace spatacs
{
namespace core
{
namespace ecs
{
    /// \todo add tags that allow passing other data,
    ///       such as IDs, handles, user data, optional components etc.

    template<class T, class... C>
    auto& find_single(std::tuple<C...>& comps)
    {
        using search_t = type_t<std::decay_t<T>>;
        using haystack_t = type_vec_t<std::decay_t<C>...>;

        constexpr std::size_t id = find(search_t{}, haystack_t{});
        return std::get<id>(comps);
    }

    template<class Input, class... Types>
    auto make_forwarder(Input&& ip, type_vec_t<Types...>)
    {
        return [&](auto&& components) {
            return ip(find_single<Types>(components)...);
        };
    };

    template<class... Types>
    struct Signature
    {
        using types_t = type_vec_t<Types...>;

        template<class Input>
        static auto forwarder(Input&& ip)
        {
            return make_forwarder(std::forward<Input>(ip), types_t{});
        }
    };

    template<class Signature>
    struct System
    {
        using signature_t = Signature;
    };
}
}
}

#endif //SPATACS_CORE_SYSTEM_H
