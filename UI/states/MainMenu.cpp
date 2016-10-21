//
// Created by erik on 10/14/16.
//

#include "UI/states/MainMenu.h"
#include <irrlicht/irrlicht.h>
#include <iostream>
#include "UI/StateManager.h"
#include "SettingsMenu.h"
#include "LevelSelection.h"

using namespace spatacs::ui;
using namespace irr;

MainMenu::MainMenu()
{
    mNextState = 0;
}

void MainMenu::startGame()
{
    mNextState = 1;
    std::cout << "StartGame\n";
}

void MainMenu::settings()
{
    mNextState = 2;
}

void MainMenu::exit()
{
    ::exit(0);
}

void MainMenu::step(StateManager& smgr)
{
    if(mNextState == 1)
    {
        smgr.setState( std::make_unique<LevelSelection>() );
    } else if(mNextState == 2)
    {
        smgr.setState( std::make_unique<SettingsMenu>() );
    }
}

void MainMenu::init(irr::IrrlichtDevice* dev)
{
    // Start Game button
    irr::core::recti bpos(300, 100, 500, 150);
    mBtn1.reset(dev->getGUIEnvironment()->addButton(bpos, 0, 1, L"Start Game"));

    // Settings button
    bpos = irr::core::recti(300, 200, 500, 250);
    mBtn2.reset(dev->getGUIEnvironment()->addButton(bpos, 0, 2, L"Settings"));

    // Exit button
    bpos = irr::core::recti(300, 300, 500, 350);
    mBtn3.reset(dev->getGUIEnvironment()->addButton(bpos, 0, 3, L"Exit"));
}

bool MainMenu::OnEvent(const irr::SEvent& event)
{
    if(event.EventType == EET_GUI_EVENT)
    {
        auto& guiev = event.GUIEvent;
        auto id = guiev.Caller->getID();
        if(guiev.EventType == gui::EGET_BUTTON_CLICKED) {
            switch(id)
            {
                case 1:
                    startGame();
                    break;
                case 2:
                    settings();
                    break;
                case 3:
                    exit();
                    break;
            }
        }
    }
    return false;
}
