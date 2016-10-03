//
// Created by erik on 9/23/16.
//

#ifndef SOI_AIPLAYER_H
#define SOI_AIPLAYER_H

#include "IUI.h"

namespace spatacs {
    namespace ui {
        class AIPlayer : public IUI
        {
        public:
            AIPlayer(std::uint64_t team);

            virtual void init() override;

            virtual std::vector<cmd::Command> getCommands() const override;

            virtual void setState(const core::GameState& state) override;

            virtual void notifyEvents(const std::vector<std::unique_ptr<events::IEvent>>& events) override;

            virtual bool step() override;

        private:
            std::uint64_t mOwnTeam;
            std::vector<cmd::Command> mCommands;

            const core::GameState* mLastState;
            std::vector<std::uint64_t> mHits;
        };
    }
}


#endif //SOI_AIPLAYER_H
