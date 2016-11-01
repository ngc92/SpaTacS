#ifndef GAMECORE_H_INCLUDED
#define GAMECORE_H_INCLUDED

#include <random>
#include <memory>

namespace spatacs
{
    namespace core
    {
        class GameInterface;
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
            void addInterface( std::shared_ptr<core::GameInterface> ui );
            void setPause(bool p);

        private:

            std::unique_ptr<GameThread> mThread;

            std::default_random_engine mRandom;

            std::shared_ptr<GameState> mState;
            std::vector<EventPtr>      mEvents;

            // interfaces
            std::vector<std::shared_ptr<core::GameInterface>> mUIs;
        };
    }
}

#endif // GAMECORE_H_INCLUDED
