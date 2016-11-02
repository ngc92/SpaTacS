//
// Created by erik on 9/23/16.
//

#ifndef SOI_AIPLAYER_H
#define SOI_AIPLAYER_H

#include "core/GameInterface.h"
#include "ObjectID.h"

namespace spatacs {
    namespace cmd
    {
        class CommandManager;
    }
    namespace game {
        class AIPlayer : public core::GameInterface
        {
        public:
            AIPlayer(std::uint64_t team, std::shared_ptr<cmd::CommandManager> cmd);

            virtual void init() override;

            /// this function gets all the events that this interface wants to transfer
            /// into the simulation.
            /// \param evts Push your events into this vector!
            void getCommandEvents(std::vector<events::EventPtr>& evts) override { }

            virtual void setState(const std::shared_ptr<const core::GameState>& state) override;

            virtual void notifyEvents(const std::vector<std::unique_ptr<events::IEvent>>& events) override;

            virtual bool step() override;

        private:
            std::uint64_t mOwnTeam;
            std::shared_ptr<cmd::CommandManager> mCommands;

            std::shared_ptr<const core::GameState> mState;
            std::vector<ObjectID> mHits;
        };
    }
}


#endif //SOI_AIPLAYER_H
