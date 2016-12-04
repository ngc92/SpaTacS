//
// Created by erik on 12/4/16.
//

#ifndef SPATACS_GAME_MAKE_SYSTEM_H
#define SPATACS_GAME_MAKE_SYSTEM_H

#include "fwd.h"

namespace spatacs
{
namespace game
{
namespace systems
{
    template<class... Args, class Lambda>
    auto make_system(Lambda&& lambda)
    {
        struct S : public core::ecs::System<core::ecs::Signature<Args...>>
        {
            S(Lambda l) : lambda(l) { }
            void operator()(Args&... args)
            {
                lambda(args...);
            }
            Lambda lambda;
        };

        return S{lambda};
    }
}
}
}

#endif //SPATACS_GAME_MAKE_SYSTEM_H
