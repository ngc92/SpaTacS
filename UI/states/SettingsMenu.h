//
// Created by erik on 10/15/16.
//

#ifndef SPATACS_SETTINGSMENU_H
#define SPATACS_SETTINGSMENU_H

#include "UI/IrrRAII.h"
#include "UI/AppState.h"

namespace irr
{
    namespace gui
    {
        class IGUIElement;
    }
}

namespace spatacs
{
    namespace ui
    {
        class SettingsMenu : public AppState
        {
        public:
            SettingsMenu();

            void step(StateManager& smgr) override;
            void init(irr::IrrlichtDevice* dev) override;
            bool OnEvent(const irr::SEvent& event) override;
        private:
            bool mBackClicked = false;
            remove_ptr<irr::gui::IGUIElement> mBackBtn;
        };
    }
}

#endif //SPATACS_SETTINGSMENU_H
