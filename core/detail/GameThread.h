//
// Created by erik on 9/22/16.
//

#ifndef SPATACS_CORE_GAME_THREAD_INCLUDED
#define SPATACS_CORE_GAME_THREAD_INCLUDED

#include <mutex>
#include <atomic>
#include <thread>
#include <chrono>

#include <boost/accumulators/accumulators.hpp>
#include <boost/accumulators/statistics/rolling_mean.hpp>

#include "core/any_vector.h"

namespace spatacs
{
namespace events
{
    class IEvent;
}
namespace core
{
    class SimulationBase;
    class GameStateBase;

    namespace detail
    {
        class GameThread
        {
        public:
            using EventPtr = std::unique_ptr<events::IEvent>;
            using EventVec = std::vector<EventPtr>;
            using StatePtr = std::unique_ptr<GameStateBase>;

            GameThread(std::unique_ptr<SimulationBase> simulation);
            ~GameThread();

            // communication
            bool has_data() const;

            StatePtr getState();
            any_vector getEvents();

            void setInEvents(EventVec events);

            // profile info
            /// returns average frame simulation time in microseconds
            double getAverageSimulationTime() const;

            // pausing
            void setPause(bool pause);

        private:
            void thread_run();

            std::unique_ptr<SimulationBase> mSimulation;

            // timing
            std::chrono::steady_clock::time_point mLastStep;
            std::chrono::milliseconds mFrameTime = std::chrono::milliseconds(100);

            // thread communication variables
            mutable std::mutex mMutex;
            std::thread mThread;
            StatePtr    mOutState;
            any_vector  mOutEvents;
            EventVec    mInEvents;
            std::atomic<bool> mHasData{false};
            std::atomic<bool> mPause;
            std::atomic<bool> mRunThread{true};

            // profiling
            boost::accumulators::accumulator_set<double,
                    boost::accumulators::features<boost::accumulators::tag::rolling_mean>> mProfileStats;
        };
    }
}
}


#endif //SPATACS_CORE_GAME_THREAD_INCLUDED
