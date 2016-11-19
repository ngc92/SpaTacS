//
// Created by erik on 11/16/16.
//

#include "GameStateBase.h"

using namespace spatacs::core;

uint64_t GameStateBase::getNextFreeID()
{
    return ++mFreeID;
}