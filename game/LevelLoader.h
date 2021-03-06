//
// Created by erik on 10/18/16.
//

#ifndef SPATACS_LEVELLOADER_H
#define SPATACS_LEVELLOADER_H

#include "core/GameInterface.h"
#include <string>
#include "events/commands.h"

namespace spatacs
{
    namespace game
    {
        class LevelLoader : public core::GameInterface
        {
        public:
            explicit LevelLoader(std::string filename);

            virtual void init() override { }

            virtual void getCommandEvents(commands_t& evts) override;

            virtual void setState(const state_t& state) override;

            virtual void notify(const notify_t& events) override { }

            virtual bool step() override;
        private:
            std::vector<events::commands_t> mSpawnEvents;
        };
    }
}

#endif //SPATACS_LEVELLOADER_H
