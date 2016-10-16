//
// Created by erik on 10/15/16.
//

#ifndef SPATACS_STATEMANAGER_H_H
#define SPATACS_STATEMANAGER_H_H

#include <memory>

namespace irr
{
    class IrrlichtDevice;
}

namespace spatacs
{
    namespace ui
    {
        class AppState;
        class EventDispatch;

        class StateManager
        {
        public:
            StateManager(irr::IrrlichtDevice* dev);
            ~StateManager();
            void setState(std::unique_ptr<AppState> st);
            void step();
        private:
            std::unique_ptr<AppState> mCurrentState;
            std::unique_ptr<EventDispatch> mEventDispatch;
            irr::IrrlichtDevice* mDevice;
        };
    }
}

#endif //SPATACS_STATEMANAGER_H_H
