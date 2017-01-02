//
// Created by erik on 1/2/17.
//

#ifndef SPATACS_GAME_STATE_H
#define SPATACS_GAME_STATE_H

#include "core/GameStateBase.h"
#include "entity/fwd.h"

namespace spatacs
{
    namespace game
    {
        class State : public core::GameStateBase
        {
        public:
            State();
            ~State();
            State(State&&);

            std::unique_ptr<core::GameStateBase> clone() const override;

            GameEntityManager& entity_manager();
            const GameEntityManager & entity_manager() const;

        private:
            State(const State&);

            struct Storage;
            std::unique_ptr<Storage> mStorage;
        };
    }
}

#endif //SPATACS_GAME_STATE_H
