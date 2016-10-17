//
// Created by erik on 9/23/16.
//

#include <irrlicht/rect.h>
#include <irrlicht/IGUIEnvironment.h>
#include <irrlicht/IGUIStaticText.h>
#include <iomanip>
#include <sstream>
#include "game/Starship.h"
#include "UnitSelection.h"
#include "UnitCommand.h"
#include <irrlicht/ICameraSceneNode.h>
#include "UI/convert.h"

using namespace spatacs;
using namespace ui;

const game::Starship* pick(const core::GameState& world, irr::core::line3df ray);

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

void UnitSelection::onRightClick(ray_t ray, const irr::SEvent::SMouseInput& event)
{
    if(mChildMode)
        mChildMode->onRightClick(ray, event);
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

void UnitSelection::init(irr::gui::IGUIEnvironment* guienv, irr::scene::ISceneManager* smgr)
{
    auto txt = guienv->addStaticText(L"Text", irr::core::recti(0, 0, 100, 20));
    txt->setOverrideColor( irr::video::SColor(255, 128, 128, 255) );
    mHoverUI = txt;
}

void UnitSelection::onKeyPress(irr::EKEY_CODE key)
{
    mKeysDown.insert(key);
    if(mChildMode)
    {
        mChildMode->onKeyPress(key);
    }
}

void UnitSelection::onKeyRelease(irr::EKEY_CODE key)
{
    mKeysDown.erase(key);
    if(mChildMode)
    {
        mChildMode->onKeyRelease(key);
    }
}

void UnitSelection::step()
{
    /// \todo make this FPS independent
    if(mKeysDown.count(irr::KEY_KEY_A))
    {
        getCamera()->setPosition( getCamera()->getPosition() + irr::core::vector3df(0, 0, 1) );
        getCamera()->setTarget( getCamera()->getTarget() + irr::core::vector3df(0, 0, 1) );
    }

    if(mKeysDown.count(irr::KEY_KEY_D))
    {
        getCamera()->setPosition( getCamera()->getPosition() - irr::core::vector3df(0, 0, 1) );
        getCamera()->setTarget( getCamera()->getTarget() - irr::core::vector3df(0, 0, 1) );
    }

    if(mKeysDown.count(irr::KEY_KEY_W))
    {
        getCamera()->setPosition( getCamera()->getPosition() + irr::core::vector3df(1, 0, 0) );
        getCamera()->setTarget( getCamera()->getTarget() + irr::core::vector3df(1, 0, 0) );
    }

    if(mKeysDown.count(irr::KEY_KEY_S))
    {
        getCamera()->setPosition( getCamera()->getPosition() - irr::core::vector3df(1, 0, 0) );
        getCamera()->setTarget( getCamera()->getTarget() - irr::core::vector3df(1, 0, 0) );
    }

    if(mKeysDown.count(irr::KEY_KEY_Q))
    {
        getCamera()->setPosition( getCamera()->getPosition() + irr::core::vector3df(0, 1, 0) );
        getCamera()->setTarget( getCamera()->getTarget() + irr::core::vector3df(0, 1, 0) );
    }

    if(mKeysDown.count(irr::KEY_KEY_Y))
    {
        getCamera()->setPosition( getCamera()->getPosition() - irr::core::vector3df(0, 1, 0) );
        getCamera()->setTarget( getCamera()->getTarget() - irr::core::vector3df(0, 1, 0) );
    }

    if(mChildMode)
        mChildMode->step();


    // Pick stuff.
    auto hover = pick(state(), mCurrentRay);
    if (hover == 0) {
        mHoverUI->setVisible(false);
    } else {
        /// \todo we are using the "actual" 3D position, but should be using the position inside the LocationPlotter3D
        auto p = convert(hover->position());
        p.Y += 5;
        auto screenpos = getScreenPosition(p);
        mHoverUI->setRelativePosition(screenpos);
        mHoverUI->setVisible(true);
        std::wstringstream ws;
        ws << std::fixed << std::setprecision(1);
        ws << L"Ship: " << hover->id() << ":\n";
        auto status = hover->shield_strength();
        ws << " " << status.current << "/" << hover->hp() << "\n";
        mHoverUI->setText(ws.str().c_str());
    }
}