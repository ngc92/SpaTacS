//
// Created by erik on 10/15/16.
//

#include "UI/AIPlayer.h"
#include "UI/IrrlichtUI.h"
#include "UI/LevelLoader.h"
#include "UI/cmd/CommandManager.h"
#include "GameState.h"
#include "core/Game.h"

using namespace spatacs::ui;

GameState::GameState() :
        mGame( std::make_unique<spatacs::core::Game>() ),
        mCmdMgr( std::make_shared<cmd::CommandManager>() )
{
    mGame->addInterface( std::make_shared<ui::AIPlayer>(2, mCmdMgr) );
    mGame->addInterface( std::make_shared<ui::LevelLoader>("level.xml") );
    mGame->addInterface( mCmdMgr );
}

GameState::~GameState()
{}


void GameState::step(StateManager& smgr)
{
    mGame->setPause(mIrrUI->pause());
    mGame->run(); /// \todo get this out of the gfx loop!
}

void GameState::init(irr::IrrlichtDevice* dev)
{
    mIrrUI = std::make_shared<ui::IrrlichtUI>(1, dev, mCmdMgr);
    mGame->addInterface( mIrrUI );
}

bool GameState::OnEvent(const irr::SEvent& event)
{
    return mIrrUI->handleUIEvent(event);
}

