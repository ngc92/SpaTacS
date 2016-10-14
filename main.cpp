#include <iostream>

#include "core/Game.h"

#include "UI/IrrlichtUI.h"
#include "UI/AIPlayer.h"
#include <irrlicht/irrlicht.h>

using namespace spatacs;

int main()
{
	core::Game game;
    irr::IrrlichtDevice* device = irr::createDevice(irr::video::EDT_OPENGL,
                                                    irr::core::dimension2du(800, 600),
                                                    32);

    game.addInterface( std::make_shared<ui::AIPlayer>(2) );
    game.addInterface( std::make_shared<ui::AIPlayer>(1) );
    auto irrlicht = std::make_shared<ui::IrrlichtUI>(1, device);
    game.addInterface( irrlicht );

	while(device->run())
	{
        game.setPause(irrlicht->pause());
        device->getVideoDriver()->beginScene();
        game.run();
        device->getVideoDriver()->endScene();
	}
}
