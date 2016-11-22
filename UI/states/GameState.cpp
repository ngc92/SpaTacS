//
// Created by erik on 10/15/16.
//

#include "game/AIPlayer.h"
#include "UI/IrrlichtUI.h"
#include "game/LevelLoader.h"
#include "cmd/CommandManager.h"
#include "GameState.h"
#include "core/Game.h"
#include "game/GameSimulation.h"

using namespace spatacs::ui;

GameState::GameState(const std::string& filename) :
        mGame( std::make_unique<spatacs::core::Game>( std::make_unique<spatacs::game::GameSimulation>() ) ),
        mCmdMgr( std::make_shared<cmd::CommandManager>() )
{
    mGame->addInterface( std::make_shared<game::AIPlayer>(2, mCmdMgr) );
    mGame->addInterface( std::make_shared<game::LevelLoader>(filename) );
    mGame->addInterface( mCmdMgr );
}

GameState::~GameState()
{}


void GameState::step(StateManager& smgr)
{
    mGame->setPause(mIrrUI->pause());
    mGame->run();
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

