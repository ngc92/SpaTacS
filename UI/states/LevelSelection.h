//
// Created by erik on 10/21/16.
//

#ifndef SPATACS_LEVELSELECTION_H
#define SPATACS_LEVELSELECTION_H

#include "UI/IrrRAII.h"
#include "UI/AppState.h"
#include <vector>
#include <string>

namespace spatacs
{
    namespace ui
    {
        class LevelSelection : public AppState
        {
        public:
            LevelSelection();

            void step(StateManager& smgr) override;
            void init(irr::IrrlichtDevice* dev) override;
            bool OnEvent(const irr::SEvent& event) override;
        private:
            int mChosenLevel = -1;
            std::vector<remove_ptr<irr::gui::IGUIElement>> mLvlBtns;
            std::vector<std::string> mLevelFiles;
        };
    }
}

#endif //SPATACS_LEVELSELECTION_H
