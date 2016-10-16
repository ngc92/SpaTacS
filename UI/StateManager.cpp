//
// Created by erik on 10/15/16.
//

#include "StateManager.h"
#include "AppState.h"
#include <irrlicht/IEventReceiver.h>
#include <irrlicht/IrrlichtDevice.h>

namespace spatacs
{
    namespace ui
    {
        struct EventDispatch : public irr::IEventReceiver
        {
            AppState* state;

            bool OnEvent(const irr::SEvent& event) override
            {
                state->OnEvent(event);
            }
        };
    }
}

using namespace spatacs::ui;

void StateManager::step()
{
    mCurrentState->step(*this);
}

void StateManager::setState(std::unique_ptr<AppState> st)
{
    mCurrentState = std::move(st);
    mCurrentState->init(mDevice);
    mEventDispatch->state = mCurrentState.get();
}

StateManager::StateManager(irr::IrrlichtDevice* dev) :
    mDevice(dev), mEventDispatch( std::make_unique<EventDispatch>() )
{
    mDevice->setEventReceiver( mEventDispatch.get() );
}

StateManager::~StateManager()
{
    mDevice->setEventReceiver(nullptr);
}
