//
// Created by erik on 9/22/16.
//

#ifndef SPATACS_COMMANDMANAGER_H_H
#define SPATACS_COMMANDMANAGER_H_H

#include "Commands.h"
#include <vector>
#include <unordered_map>
#include <boost/optional.hpp>
#include <events/IEvent.h>
#include "UI/IUI.h"

namespace spatacs
{
    namespace core
    {
        class GameState;
    }
    namespace cmd
    {
        struct CommandSlot
        {
            Move move;
            boost::optional<Attack> attack;
            bool delflag;
        };

        class CommandManager : public ui::IUI
        {
        public:
            void addCommand( std::uint64_t target, Command cmd );

            const CommandSlot& getCommandsOf( std::uint64_t ship ) const;


            //
            void init() override { };
            void getCommandEvents(std::vector<events::EventPtr>& evts) override;
            void setState(const std::shared_ptr<const core::GameState>& state) override;
            void notifyEvents(const std::vector<std::unique_ptr<events::IEvent>>& events) override { }
            bool step() override { return true; }

        private:
            void validate( );

            std::unordered_map<std::uint64_t, CommandSlot> mCommandSlots;
            std::vector<SetWpnMode> mOneShotCommands;
            std::shared_ptr<const core::GameState> mState;
        };
    }
}

#endif //SOI_COMMANDMANAGER_H_H
