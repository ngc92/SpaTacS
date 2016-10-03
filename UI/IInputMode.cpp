//
// Created by erik on 9/22/16.
//

#include "IInputMode.h"
#include "IrrlichtUI.h"
#include <irrlicht/irrlicht.h>

using namespace spatacs;
const core::GameState& ui::IInputMode::state() const
{
    return mMainUI->state();
}

void ui::IInputMode::addCommand(cmd::Command c)
{
    mMainUI->addCommand(c);
}

void ui::IInputMode::setMainUI(ui::IrrlichtUI* mainui)
{
    mMainUI = mainui;
    init(mMainUI->getDevice()->getGUIEnvironment());
}

irr::core::vector2di ui::IInputMode::getScreenPosition(const irr::core::vector3df pos3d)
{
    return mMainUI->getDevice()->getSceneManager()->getSceneCollisionManager()->getScreenCoordinatesFrom3DPosition(pos3d);
}

irr::scene::ICameraSceneNode* ui::IInputMode::getCamera()
{
    return getMainUI()->getDevice()->getSceneManager()->getActiveCamera();
}
