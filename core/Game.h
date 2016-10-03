#ifndef GAMECORE_H_INCLUDED
#define GAMECORE_H_INCLUDED

#include "UI/cmd/Commands.h"
#include <random>
#include <memory>
#include "GameState.h"

namespace spatacs
{
    namespace ui
    {
        class IUI;
    }
    namespace events
    {
        class IEvent;
    }
	namespace core {
        class GameState;
        class GameThread;

        class Game {
        public:
            using EventPtr = std::unique_ptr<events::IEvent>;
            Game();
            ~Game();

            void run();
            void addInterface( std::shared_ptr<ui::IUI> ui );

        private:

            std::unique_ptr<GameThread> mThread;

            std::default_random_engine mRandom;

            GameState mState;
            std::vector<EventPtr> mEvents;

            // interfaces
            std::vector<std::shared_ptr<ui::IUI>> mUIs;
        };
    }
}

#endif // GAMECORE_H_INCLUDED
