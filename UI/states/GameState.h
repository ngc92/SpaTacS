//
// Created by erik on 10/15/16.
//

#ifndef SPATACS_GAMESTATE_H
#define SPATACS_GAMESTATE_H

#include "UI/AppState.h"
#include <memory>

namespace irr
{
    class IrrlichtDevice;
}

namespace spatacs
{
    class IrrlichtUI;
    namespace core
    {
        class Game;
    }
    namespace ui
    {
        class GameState : public AppState
        {
        public:
            GameState();
            ~GameState();
            void step(StateManager& smgr) override;
            void init(irr::IrrlichtDevice* dev) override;
            bool OnEvent(const irr::SEvent& event) override;
        private:
            std::unique_ptr<core::Game> mGame;
            std::shared_ptr<IrrlichtUI> mIrrUI;
        };
    }
}

#endif //SPATACS_GAMESTATE_H
