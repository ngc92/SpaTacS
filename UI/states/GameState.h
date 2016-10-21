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
    namespace cmd
    {
        class CommandManager;
    }
    namespace core
    {
        class Game;
    }
    namespace ui
    {
        class GameState : public AppState
        {
        public:
            GameState(const std::string& filename);
            ~GameState();
            void step(StateManager& smgr) override;
            void init(irr::IrrlichtDevice* dev) override;
            bool OnEvent(const irr::SEvent& event) override;
        private:
            std::unique_ptr<core::Game> mGame;

            std::shared_ptr<IrrlichtUI>          mIrrUI;
            std::shared_ptr<cmd::CommandManager> mCmdMgr;
        };
    }
}

#endif //SPATACS_GAMESTATE_H
