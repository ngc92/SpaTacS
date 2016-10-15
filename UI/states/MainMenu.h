//
// Created by erik on 10/14/16.
//

#ifndef SPATACS_MAINMENU_H
#define SPATACS_MAINMENU_H

#include "UI/AppState.h"
#include "UI/IrrRAII.h"

namespace irr
{
    class IrrlichtDevice;
    namespace gui
    {
        class IGUIElement;
    }
}

namespace spatacs
{
    namespace ui
    {
        class MainMenu : public AppState
        {
        public:
            MainMenu();

            void step(StateManager& smgr) override;
            void init(irr::IrrlichtDevice* dev) override;
            bool OnEvent(const irr::SEvent& event) override;

            void startGame();
            void settings();
            void exit();
        private:
            unsigned mNextState;
            remove_ptr<irr::gui::IGUIElement> mBtn1;
            remove_ptr<irr::gui::IGUIElement> mBtn2;
            remove_ptr<irr::gui::IGUIElement> mBtn3;
        };
    }
}

#endif //SPATACS_MAINMENU_H
