//
// Created by erik on 10/18/16.
//

#ifndef SPATACS_LEVELLOADER_H
#define SPATACS_LEVELLOADER_H

#include "core/GameInterface.h"
#include <string>
#include "events/Spawn.h"

namespace spatacs
{
    namespace game
    {
        class LevelLoader : public core::GameInterface
        {
        public:
            explicit LevelLoader(std::string filename);

            virtual void init() override { }

            virtual void getCommandEvents(std::vector<spatacs::events::EventPtr>& evts) override;

            virtual void setState(const state_t& state) override;

            virtual void notify(const notify_t& events) override { }

            virtual bool step() override;
        private:
            std::vector<spatacs::events::SpawnShip> mSpawnEvents;
        };
    }
}

#endif //SPATACS_LEVELLOADER_H
