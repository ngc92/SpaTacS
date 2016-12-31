//
// Created by erik on 9/23/16.
//

#ifndef SOI_AIPLAYER_H
#define SOI_AIPLAYER_H

#include "core/TypedGameInterface.h"
#include "ObjectID.h"
#include "Starship.h"
#include "game/events/events.h"

namespace spatacs
{
    namespace cmd
    {
        class CommandManager;
    }
    namespace game
    {
        class Shield;

        class AIPlayer : public core::TypedGameInterface<game::GameState, game::events::notification_t>
        {
        public:
            AIPlayer(std::uint64_t team, std::shared_ptr<cmd::CommandManager> cmd);

            virtual void init() override;

            /// this function gets all the events that this interface wants to transfer
            /// into the simulation.
            /// \param evts Push your events into this vector!
            void getCommandEvents(std::vector<spatacs::events::EventPtr>& evts) override { }

            virtual void setState(state_t state) override;

            virtual bool step() override;

        private:
            std::uint64_t mOwnTeam;
            std::shared_ptr<cmd::CommandManager> mCommands;

            std::shared_ptr<const game::GameState> mState;

            struct BestAmmo
            {
                std::string ammo;
                double score;
            };
            BestAmmo getBestAmmo(const spatacs::game::Starship& own, const Starship& target, double target_shield) const;
        };
    }
}


#endif //SOI_AIPLAYER_H
