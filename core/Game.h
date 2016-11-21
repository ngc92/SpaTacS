#ifndef SPATACS_CORE_GAME_H_INCLUDED
#define SPATACS_CORE_GAME_H_INCLUDED

#include <random>
#include <memory>
#include <functional>

namespace spatacs
{
    namespace events
    {
        class IEvent;
    }
	namespace core {
        class GameStateBase;
        namespace detail {
            class GameThread;
        }
        class GameInterface;
        class SimulationBase;

        class Game {
        public:
            using EventPtr = std::unique_ptr<events::IEvent>;
            //! creates a game.
            //! \param simulation: Pointer to the simulation that is to be used in the game thread.
            Game(std::unique_ptr<SimulationBase> simulation);
            ~Game();

            void run();
            void addInterface( std::shared_ptr<GameInterface> ui );
            void setPause(bool p);

        private:

            std::unique_ptr<detail::GameThread> mThread;

            std::default_random_engine     mRandom;

            std::shared_ptr<GameStateBase> mState;
            std::vector<EventPtr>          mEvents;

            // interfaces
            std::vector<std::shared_ptr<GameInterface>> mUIs;
        };
    }
}

#endif // SPATACS_CORE_GAME_H_INCLUDED
