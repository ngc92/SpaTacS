//
// Created by erik on 9/23/16.
//

#ifndef SOI_AIPLAYER_H
#define SOI_AIPLAYER_H

#include "cmd/CommandManager.h"
#include "IUI.h"

namespace spatacs {
    namespace ui {
        class AIPlayer : public IUI
        {
        public:
            AIPlayer(std::uint64_t team);

            virtual void init() override;

            /// this function gets all the events that this interface wants to transfer
            /// into the simulation.
            /// \param evts Push your events into this vector!
            void getCommandEvents(std::vector<events::EventPtr>& evts) const override;

            virtual void setState(const core::GameState& state) override;

            virtual void notifyEvents(const std::vector<std::unique_ptr<events::IEvent>>& events) override;

            virtual bool step() override;

        private:
            std::uint64_t mOwnTeam;
            cmd::CommandManager mCommands;

            const core::GameState* mLastState;
            std::vector<std::uint64_t> mHits;
        };
    }
}


#endif //SOI_AIPLAYER_H
