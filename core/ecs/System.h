//
// Created by erik on 11/20/16.
//

#ifndef SPATACS_CORE_SYSTEM_H
#define SPATACS_CORE_SYSTEM_H

#include <utility>
#include <tuple>
#include "core/meta/types.h"

namespace spatacs
{
namespace core
{
namespace ecs
{
    template<class... Types>
    struct Signature
    {
        using types_t = meta::pack_t<Types...>;
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
