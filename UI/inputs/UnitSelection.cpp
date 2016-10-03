//
// Created by erik on 9/23/16.
//

#include <irrlicht/rect.h>
#include <irrlicht/IGUIEnvironment.h>
#include <irrlicht/IGUIStaticText.h>
#include <iomanip>
#include <sstream>
#include "core/Starship.h"
#include "UnitSelection.h"
#include "UnitCommand.h"
#include <irrlicht/ICameraSceneNode.h>

using namespace spatacs;
using namespace ui;

const spatacs::core::Starship* pick(const spatacs::core::GameState& world, irr::core::line3df ray);

UnitSelection::UnitSelection(std::uint64_t team) :
        mOwnTeam(team)
{

}

void UnitSelection::onLeftClick(IInputMode::ray_t ray)
{
    auto sel = pick(state(), ray);
    if(sel && sel->team() == mOwnTeam)
    {
        mChildMode = std::make_shared<UnitCommand>( sel->id() );
        mChildMode->setMainUI( getMainUI() );
    } else
    {
        mChildMode.reset();
    }
}

void UnitSelection::onMouseMove(IInputMode::ray_t ray)
{
    mCurrentRay = ray;
    if(mChildMode)
        mChildMode->onMouseMove(ray);
}

void UnitSelection::onRightClick(IInputMode::ray_t ray)
{
    if(mChildMode)
        mChildMode->onRightClick(ray);
}

void UnitSelection::onWheel(float scroll)
{
    if(mChildMode) {
        mChildMode->onWheel(scroll);
    } else {
        auto delta = getCamera()->getTarget() - getCamera()->getPosition();
        delta.normalize();
        getCamera()->setPosition(getCamera()->getPosition() + delta*scroll);
    }
}

void UnitSelection::draw(irr::video::IVideoDriver* driver)
{
    if(mChildMode)
        mChildMode->draw(driver);

    // Pick stuff.
    auto hover = pick(state(), mCurrentRay);
    if (hover == 0) {
        mHoverUI->setVisible(false);
    } else {
        /// \todo we are using the "actual" 3D position, but should be using the position inside the LocationPlotter3D
        irr::core::vector3df p = {hover->position().x.value, hover->position().y.value, hover->position().z.value};
        p *= 20;
        p.Y += 5;
        auto screenpos = getScreenPosition(p);
        mHoverUI->setRelativePosition(screenpos);
        mHoverUI->setVisible(true);
        std::wstringstream ws;
        ws << std::fixed << std::setprecision(1);
        ws << L"Ship: " << hover->id() << ":\n";
        auto status = hover->shield_strength();
        ws << " " << status.current << "/" << hover->HP() << "\n";
        mHoverUI->setText(ws.str().c_str());
    }

}

void UnitSelection::init(irr::gui::IGUIEnvironment* guienv)
{
    auto txt = guienv->addStaticText(L"Text", irr::core::recti(0, 0, 100, 20));
    txt->setOverrideColor( irr::video::SColor(255, 128, 128, 255) );
    mHoverUI = txt;
}

void UnitSelection::onKeyPress(irr::EKEY_CODE key)
{
    if(key == irr::KEY_KEY_A)
    {
        getCamera()->setPosition( getCamera()->getPosition() + irr::core::vector3df(0, 0, 1) );
        getCamera()->setTarget( getCamera()->getTarget() + irr::core::vector3df(0, 0, 1) );
    } else

    if(key == irr::KEY_KEY_D)
    {
        getCamera()->setPosition( getCamera()->getPosition() - irr::core::vector3df(0, 0, 1) );
        getCamera()->setTarget( getCamera()->getTarget() - irr::core::vector3df(0, 0, 1) );
    } else

    if(key == irr::KEY_KEY_W)
    {
        getCamera()->setPosition( getCamera()->getPosition() + irr::core::vector3df(1, 0, 0) );
        getCamera()->setTarget( getCamera()->getTarget() + irr::core::vector3df(1, 0, 0) );
    } else

    if(key == irr::KEY_KEY_S)
    {
        getCamera()->setPosition( getCamera()->getPosition() - irr::core::vector3df(1, 0, 0) );
        getCamera()->setTarget( getCamera()->getTarget() - irr::core::vector3df(1, 0, 0) );
    } else

    if(key == irr::KEY_KEY_Q)
    {
        getCamera()->setPosition( getCamera()->getPosition() + irr::core::vector3df(0, 1, 0) );
        getCamera()->setTarget( getCamera()->getTarget() + irr::core::vector3df(0, 1, 0) );
    } else

    if(key == irr::KEY_KEY_Y)
    {
        getCamera()->setPosition( getCamera()->getPosition() - irr::core::vector3df(0, 1, 0) );
        getCamera()->setTarget( getCamera()->getTarget() - irr::core::vector3df(0, 1, 0) );
    } else if(mChildMode)
    {
        mChildMode->onKeyPress(key);
    }
}