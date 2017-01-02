//
// Created by erik on 11/16/16.
//

#ifndef SPATACS_CORE_GAMESTATEBASE_H
#define SPATACS_CORE_GAMESTATEBASE_H

#include <cstdint>
#include <memory>

namespace spatacs
{
namespace core
{
    class GameStateBase
    {
    public:
        virtual ~GameStateBase() = default;

        virtual std::unique_ptr<GameStateBase> clone() const = 0;
    };
}
}

#endif //SPATACS_CORE_GAMESTATEBASE_H
