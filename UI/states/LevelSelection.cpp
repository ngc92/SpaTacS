//
// Created by erik on 10/21/16.
//

#include "LevelSelection.h"
#include "GameState.h"
#include "UI/StateManager.h"
#include "MainMenu.h"
#include <boost/filesystem.hpp>
#include <irrlicht/rect.h>
#include <irrlicht/IrrlichtDevice.h>
#include <irrlicht/IGUIEnvironment.h>
#include <irrlicht/IGUIElement.h>
#include <irrlicht/IGUIButton.h>

using namespace spatacs;
using namespace ui;

LevelSelection::LevelSelection()
{

}

void LevelSelection::step(StateManager& smgr)
{
    if(mChosenLevel == -2)
    {
        smgr.setState(std::make_unique<MainMenu>( ));
    }
    else if(mChosenLevel != -1)
    {
        smgr.setState(std::make_unique<GameState>( mLevelFiles.at(mChosenLevel) ));
    }
}

void LevelSelection::init(irr::IrrlichtDevice* dev)
{
    boost::filesystem::path p("data/scenarios");
    boost::filesystem::directory_iterator it(p);
    unsigned y = 0;
    unsigned i = 1;
    for(auto& f : it)
    {
        auto name = f.path().stem();
        irr::core::recti bpos(300, 100 + y, 500, 140 + y);
        irr::gui::IGUIElement* new_btn = dev->getGUIEnvironment()->addButton(bpos, 0, i, irr::core::stringw(name.c_str()).c_str());
        y += 50;
        i += 1;
        mLvlBtns.push_back( remove_ptr<irr::gui::IGUIElement>(new_btn) );
        mLevelFiles.push_back( f.path().native().c_str() );
    }
    irr::core::recti bpos(300, 100 + y, 500, 140 + y);
    irr::gui::IGUIElement* new_btn = dev->getGUIEnvironment()->addButton(bpos, 0, -1, L"Back");
    mLvlBtns.push_back( remove_ptr<irr::gui::IGUIElement>(new_btn) );
}

bool LevelSelection::OnEvent(const irr::SEvent& event)
{
    using namespace irr;
    if(event.EventType == EET_GUI_EVENT)
    {
        auto& guiev = event.GUIEvent;
        auto id = guiev.Caller->getID();
        if(guiev.EventType == gui::EGET_BUTTON_CLICKED) {
            mChosenLevel = id - 1;
        }
    }
    return false;
}