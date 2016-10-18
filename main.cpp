#include <iostream>

#include <irrlicht/irrlicht.h>
#include <fenv.h>
#include "UI/states/MainMenu.h"
#include "UI/states/GameState.h"
#include "UI/StateManager.h"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

using namespace spatacs;

int main()
{
    feenableexcept(FE_INVALID | FE_OVERFLOW);

    boost::property_tree::ptree settings;
    boost::property_tree::xml_parser::read_xml("settings.xml", settings);

    irr::IrrlichtDevice* device = irr::createDevice(irr::video::EDT_OPENGL,
                                                    irr::core::dimension2du(settings.get<uint16_t>("settings.width", 800),
                                                                            settings.get<uint16_t>("settings.height", 600)),
                                                    32,
                                                    settings.get<bool>("settings.fullscreen", false));

    ui::StateManager statemgr(device);
    statemgr.setState(std::make_unique<ui::MainMenu>());
    
	while(device->run())
	{
        statemgr.step();
        device->getVideoDriver()->beginScene();
        device->getSceneManager()->drawAll();
        device->getGUIEnvironment()->drawAll();
        device->getVideoDriver()->endScene();
	}
}
