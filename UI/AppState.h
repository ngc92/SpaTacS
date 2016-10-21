//
// Created by erik on 10/15/16.
//

#ifndef SPATACS_APPSTATE_H
#define SPATACS_APPSTATE_H

namespace irr
{
    class IrrlichtDevice;
    class SEvent;
    namespace gui
    {
        class IGUIElement;
    }
}

namespace spatacs
{
    namespace ui
    {
        class StateManager;
        class AppState
        {
        public:
            virtual ~AppState() = default;
            virtual void step(StateManager& smgr) = 0;
            virtual void init(irr::IrrlichtDevice* dev) = 0;
            virtual bool OnEvent(const irr::SEvent& event) = 0;
        };
    }
}

#endif //SPATACS_APPSTATE_H
