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
        std::uint64_t mFreeID = 0;
    public:
        virtual ~GameStateBase() = default;

        std::uint64_t getNextFreeID();
        virtual void cleanup() = 0;
        virtual std::unique_ptr<GameStateBase> clone() const = 0;
    };
}
}

#endif //SPATACS_CORE_GAMESTATEBASE_H
