//
// Created by erik on 9/22/16.
//

#ifndef SOI_COMMANDMANAGER_H_H
#define SOI_COMMANDMANAGER_H_H

#include "Commands.h"
#include <vector>
#include <unordered_map>
#include <boost/optional.hpp>

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
            boost::optional<Move> move;
            boost::optional<Attack> attack;
        };

        class CommandManager
        {
        public:
            void validate( const core::GameState& state );
            void addCommand( Command cmd );
            std::vector<Command> getCommands() const;
        private:
            std::unordered_map<std::uint64_t, CommandSlot> mCommandSlots;
            std::vector<SetWpnMode> mOneShotCommands;
        };
    }
}

#endif //SOI_COMMANDMANAGER_H_H
