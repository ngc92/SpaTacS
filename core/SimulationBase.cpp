//
// Created by erik on 10/1/16.
//

#include <iostream>
#include "SimulationBase.h"
#include "GameStateBase.h"

using namespace spatacs;
using namespace core;

auto SimulationBase::step(EventVec inEvents) -> EventVec
{
    cleanup();

    // process events generated due to commands
    mEventCache = move(inEvents);
    eventLoop();
    update();

    return std::move(mAllEvents);
}

void SimulationBase::cleanup()
{
    mAllEvents.clear();
    mState->cleanup();
}

void SimulationBase::addEvent(EventPtr e)
{
    mEventCache.push_back( move(e) );
}

SimulationBase::SimulationBase()
{
}

SimulationBase::~SimulationBase()
{
}

auto SimulationBase::extractState() const -> StatePtr
{
    return mState->clone();
}
