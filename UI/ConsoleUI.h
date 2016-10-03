//
// Created by erik on 9/19/16.
//

#ifndef SOI_CONSOLEUI_H
#define SOI_CONSOLEUI_H

#include "IUI.h"
#include <thread>
#include <mutex>
#include "cmd/CommandManager.h"

namespace spatacs
{
    namespace ui
    {
        class ConsoleUI : public IUI
        {
        public:
            virtual void init() override;
            virtual std::vector<cmd::Command> getCommands() const override;
            virtual void setState(const core::GameState& state) override;

            bool step() override;

            static void printState( const core::GameState& state );

            virtual void notifyEvents(const std::vector<std::unique_ptr<events::IEvent>>& events) override;

        private:
            void readLoop();
            void addCommand(cmd::Command c);

            std::thread mConsoleThread;

            mutable std::mutex mCmdMutex;
            core::GameState mState;
            mutable std::mutex mStateMutex;

            cmd::CommandManager mCommandManager;

        };
    }
}

#endif //SOI_CONSOLEUI_H
