#include <iostream>

#include "core/Game.h"

#include "UI/IrrlichtUI.h"
#include "UI/AIPlayer.h"

using namespace spatacs;

int main()
{
	core::Game game;

    game.addInterface( std::make_shared<ui::AIPlayer>(2) );
    auto irrlicht = std::make_shared<ui::IrrlichtUI>(1);
    game.addInterface( irrlicht );

	while(true)
	{
        game.run();
	}
}
