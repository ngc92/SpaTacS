//
// Created by erik on 9/22/16.
//

#include "GameThread.h"
#include "events/IEvent.h"
#include "events/Combat.h"
#include "physics/HitTests.h"
#include <cassert>
#include <iostream>
#include "Simulation.h"

using namespace spatacs;
using namespace core;

namespace chrono = std::chrono;
using chrono::steady_clock;

GameThread::GameThread() : mSimulation( std::make_unique<Simulation>() ),
                           mLastStep( steady_clock::now() ),
                           mProfileStats( boost::accumulators::tag::rolling_window::window_size = 100 ),
                           mPause( false ), mRunThread(true)
{
    mThread = std::thread([this](){ this->thread_run(); });
}

GameThread::~GameThread()
{
    mRunThread = false;
    mThread.join();
}

void GameThread::thread_run()
{
    while(mRunThread)
    {
        auto now = steady_clock::now();
        // check that we are ready for the next frame: last data has been processed, and enough time has passed
        if( !has_data() && now - mLastStep > mFrameTime &&!mPause )
        {
            mLastStep += mFrameTime;
            assert(mHasData == false);
            EventVec inev;
            {
                std::lock_guard<std::mutex> lck(mMutex);
                inev = std::move(mInEvents);
            }

            auto start_profile = steady_clock::now();
            auto events   = mSimulation->step(std::move(inev));
            auto newstate = mSimulation->extractState();
            auto delta_t  = steady_clock::now() - start_profile;

            mProfileStats( chrono::duration_cast<chrono::microseconds>(delta_t).count() );

            // update the output variables
            {
                std::lock_guard<std::mutex> lck(mMutex);
                mOutState = std::move(newstate);    // now just move the state, this is fast.
                mOutEvents = std::move(events);
            }
            mHasData = true;
        }
        std::this_thread::yield();
    }
}

bool GameThread::has_data() const
{
    return mHasData;
}

GameState GameThread::getState()
{
    assert(mHasData);
    std::lock_guard<std::mutex> lck(mMutex);
    return std::move(mOutState);
}

auto GameThread::getEvents() -> EventVec
{
    assert(mHasData);
    std::lock_guard<std::mutex> lck(mMutex);
    return std::move(mOutEvents);
}

void GameThread::setInEvents(GameThread::EventVec events)
{
    assert(mHasData);
    std::lock_guard<std::mutex> lck(mMutex);
    mInEvents = std::move(events);
    mHasData = false;
}

double GameThread::getAverageSimulationTime() const {
    return boost::accumulators::rolling_mean( mProfileStats );
}

void GameThread::setPause(bool pause)
{
    if(mPause != pause) {
        mPause = pause;
        mLastStep = steady_clock::now();
    }
}

