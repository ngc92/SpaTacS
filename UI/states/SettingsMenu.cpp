//
// Created by erik on 10/15/16.
//

#include "SettingsMenu.h"
#include "MainMenu.h"
#include <irrlicht/irrlicht.h>

using namespace spatacs::ui;

SettingsMenu::SettingsMenu()
{}

void SettingsMenu::step(StateManager& smgr)
{

}

void SettingsMenu::init(irr::IrrlichtDevice* dev)
{
    // Back button
    auto bpos = irr::core::recti(300, 300, 500, 350);
    mBackBtn.reset(dev->getGUIEnvironment()->addButton(bpos, 0, 3, L"Back"));
}

bool SettingsMenu::OnEvent(const irr::SEvent& event)
{
    if(event.EventType == irr::EET_GUI_EVENT)
    {
        auto& guiev = event.GUIEvent;
        auto id = guiev.Caller->getID();
        if(guiev.EventType == irr::gui::EGET_BUTTON_CLICKED) {
            switch(id)
            {
                case 3:
                    mBackClicked = true;
                    break;
            }
        }
    }
    return false;
}

