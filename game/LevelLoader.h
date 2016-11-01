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
            LevelLoader(std::string filename);

            virtual void init() override { }

            virtual void getCommandEvents(std::vector<events::EventPtr>& evts) override;

            virtual void setState(const std::shared_ptr<const core::GameState>& state) override;

            virtual void notifyEvents(const std::vector<std::unique_ptr<events::IEvent>>& events) override { }

            virtual bool step() override;
        private:
            std::vector<events::SpawnShip> mSpawnEvents;
        };
    }
}

#endif //SPATACS_LEVELLOADER_H