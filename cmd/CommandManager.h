//
// Created by erik on 9/22/16.
//

#ifndef SPATACS_COMMANDMANAGER_H
#define SPATACS_COMMANDMANAGER_H

#include <vector>
#include <unordered_map>
#include <boost/optional.hpp>
#include <events/IEvent.h>
#include "game/ObjectID.h"
#include "core/GameInterface.h"
#include "Move.h"
#include "Attack.h"
#include "SetMode.h"
#include <boost/variant.hpp>

namespace spatacs
{
    namespace game
    {
        class State;
    }
    namespace cmd
    {
        struct CommandSlot
        {
            Move move;
            boost::optional<Attack> attack;
            bool delflag;
        };

        class CommandManager : public core::GameInterface
        {
        public:
            void addCommand(game::ObjectID target, Move cmd);
            void addCommand(game::ObjectID target, Attack cmd);
            void addCommand(game::ObjectID target, SetWpnMode cmd);
            void addCommand(game::ObjectID target, SetSystemActivity cmd);

            const CommandSlot& getCommandsOf(game::ObjectID ship) const;

            //
            void init() override { };
            void getCommandEvents(std::vector<events::EventPtr>& evts) override;
            void setState(const state_t& state) override;
            void notify(const notify_t& events) override { }
            bool step() override { return true; }

        private:
            void validate( );

            using oneshot_t = boost::variant<SetWpnMode, SetSystemActivity> ;

            std::unordered_map<game::ObjectID, CommandSlot> mCommandSlots;
            std::vector<oneshot_t> mOneShotCommands;
            std::shared_ptr<const game::State> mState;
        };
    }
}

#endif //SPATACS_COMMANDMANAGER_H
