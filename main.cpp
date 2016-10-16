#include <iostream>

#include <irrlicht/irrlicht.h>
#include <fenv.h>
#include "UI/states/MainMenu.h"
#include "UI/states/GameState.h"
#include "UI/StateManager.h"

using namespace spatacs;

int main()
{
    feenableexcept(FE_INVALID | FE_OVERFLOW);

    irr::IrrlichtDevice* device = irr::createDevice(irr::video::EDT_OPENGL,
                                                    irr::core::dimension2du(800, 600),
                                                    32);

    ui::StateManager statemgr(device);
    statemgr.setState(std::make_unique<ui::MainMenu>());

	while(device->run())
	{
        device->getVideoDriver()->beginScene();
        device->getSceneManager()->drawAll();
        device->getGUIEnvironment()->drawAll();
        statemgr.step(); /// \todo get this out of the gfx loop!
        device->getVideoDriver()->endScene();
	}
}
