//
// Created by erik on 9/22/16.
//

#ifndef SOI_GAMETHREAD_H
#define SOI_GAMETHREAD_H

#include "GameState.h"
#include <mutex>
#include <atomic>
#include <thread>
#include <chrono>

#include <boost/accumulators/accumulators.hpp>
#include <boost/accumulators/statistics/rolling_mean.hpp>

namespace spatacs
{
namespace events
{
    class IEvent;
}
namespace core
{
    class Simulation;
    class GameThread
    {
    public:
        using EventPtr = std::unique_ptr<events::IEvent>;
        using EventVec = std::vector<EventPtr>;
        GameThread();
        ~GameThread();

        // communication
        bool has_data() const;
        GameState getState();
        EventVec getEvents();
        void setInEvents( EventVec events );

        // profile info
        /// returns average frame simulation time in microseconds
        double getAverageSimulationTime() const;

        // pausing
        void setPause( bool pause );
    private:
        void thread_run();

        std::unique_ptr<Simulation> mSimulation;

        // timing
        std::chrono::steady_clock::time_point mLastStep;
        std::chrono::milliseconds mFrameTime = std::chrono::milliseconds(100);

        // thread communication variables
        mutable std::mutex mMutex;
        std::thread mThread;
        GameState mOutState;
        EventVec mOutEvents;
        EventVec mInEvents;
        std::atomic<bool> mHasData;
        std::atomic<bool> mPause;
        std::atomic<bool> mRunThread;

        // profiling
        boost::accumulators::accumulator_set<double, boost::accumulators::features<boost::accumulators::tag::rolling_mean>> mProfileStats;
    };
}
}


#endif //SOI_GAMETHREAD_H
