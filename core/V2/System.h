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
    auto find_single(std::tuple<C...>& comps)
    {
        constexpr std::size_t id = find(type_t<T>{}, type_vec_t<C...>{});
        return std::get<id>(comps);
    }

    template<class Input, class... Types>
    auto make_forwarder(Input&& ip, type_vec_t<Types...>)
    {
        return [&](auto&& bits, auto&& components){
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
    class System
    {

    };
}
}
}

#endif //SPATACS_CORE_SYSTEM_H
