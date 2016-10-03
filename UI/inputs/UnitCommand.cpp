//
// Created by erik on 9/22/16.
//

#include "UI/cmd/Attack.h"
#include "UI/cmd/Move.h"
#include <irrlicht/SMaterial.h>
#include <irrlicht/IVideoDriver.h>
#include <irrlicht/IGUIEnvironment.h>
#include <irrlicht/IGUIStaticText.h>
#include <sstream>
#include <iomanip>
#include "UnitCommand.h"
#include "core/GameState.h"
#include "core/components/IWeapon.h"
#include <irrlicht/ICameraSceneNode.h>
#include <iostream>

using namespace spatacs;

/// \todo this is copy past, improve that!
const spatacs::core::Starship* pick(const spatacs::core::GameState& world, irr::core::line3df ray);

ui::UnitCommand::UnitCommand(std::uint64_t id) :
        mActiveShipID(id)
{

}

void ui::UnitCommand::init(irr::gui::IGUIEnvironment* guienv)
{
    mTargetY = state().getShip(mActiveShipID).position().y;
    auto txt = guienv->addStaticText(L"", irr::core::recti(0, 0, 100, 40));
    txt->setOverrideColor( irr::video::SColor(255, 128, 128, 255) );
    mDistanceMarker = txt;

    txt = guienv->addStaticText(L"", irr::core::recti(10, 10, 100, 90));
    txt->setOverrideColor( irr::video::SColor(255, 128, 128, 255) );
    mShipInfo = txt;

    txt = guienv->addStaticText(L"", irr::core::recti(700, 10, 790, 70));
    txt->setOverrideColor( irr::video::SColor(255, 128, 128, 255) );
    mTargetInfo = txt;
}


ui::UnitCommand::~UnitCommand()
{
    if(mDistanceMarker)
        mDistanceMarker->remove();
    if(mShipInfo)
        mShipInfo->remove();
    if(mTargetInfo)
        mTargetInfo->remove();
}

void ui::UnitCommand::onRightClick(ray_t ray)
{
    auto& ship = state().getShip(mActiveShipID);
    auto t = pick(state(), ray);
    if( t != 0 )
    {
        if(t->team() != ship.team())
        {
            addCommand(cmd::Attack(mActiveShipID, t->id()));
        }
    }else {
        auto target = aim(ray);
        if(target) {
            auto vec = target.get();
            addCommand(cmd::Move(mActiveShipID, vec.x, vec.y, vec.z, spatacs::speed_t(10)));
        }
    }
}

void ui::UnitCommand::draw(irr::video::IVideoDriver* driver)
{
    mBaseY = std::round(getCamera()->getTarget().Y / 10.0) * 10.0_km;

    using namespace irr;
    using irr::core::vector3df;

    // ship info text
    auto& ship = state().getShip(mActiveShipID);
    {
        std::wstringstream stream;
        stream << std::fixed << std::setprecision(1);
        stream << ship.name().c_str() << ":\n";
        stream << " shield: " << ship.shield_strength().current << "/" << ship.shield_strength().max << "\n";
        stream << " hull: " << ship.hull_status().current << "/" << ship.hull_status().max << "\n";
        stream << " hp: "     << ship.HP() << "\n";
        stream << " egy: " << ship.usedEnergy() / 0.1f << "/" << ship.producedEnergy() / 0.1f << "\n";
        stream << " mode: " << ship.weapon(0).mode() << "\n";
        mShipInfo->setText( stream.str().c_str() );
    }

    if(mMode == MOVE)
    {
        mTargetInfo->setVisible(false);
    } else
    {
        auto& target = state().getShip(mCurrentAimShip);
        std::wstringstream stream;
        stream << std::fixed << std::setprecision(1);
        stream << target.name().c_str() << ":\n";
        stream << " shield: " << target.shield_strength().current << "/" << target.shield_strength().max << "\n";
        stream << " hull: "   << target.hull_status().current     << "/" << target.hull_status().max << "\n";
        stream << " hp: "     << target.HP() << "\n";
        auto dst = distance(ship, target);
        stream << " hit: "  << ship.weapon(0).hit_chance(dst, target.radius().value * target.radius().value) * 100 << "%\n";
        mTargetInfo->setText( stream.str().c_str() );
        mTargetInfo->setVisible(true);
    }

    if (mCurrentAim) {
        vector3df sp{ship.position().x.value, ship.position().y.value, ship.position().z.value};
        sp *= 20;

        auto caim = mCurrentAim.get();
        vector3df aim{caim.x.value, caim.y.value, caim.z.value};
        aim *= 20;
        auto aimbase = aim;
        aimbase.Y = std::round(getCamera()->getTarget().Y / 10) * 10;
        irr::core::line3df flightline(sp, aim);

        std::wstringstream stream;
        stream << std::fixed << std::setprecision(1) << flightline.getLength() / 20 << "km\n";
        if(mMode == ATTACK)
        {
            auto& target = state().getShip(mCurrentAimShip);
            auto dst = distance(ship, target);
            stream << " hit: "  << ship.weapon(0).hit_chance(dst, target.radius().value * target.radius().value) * 100 << "%\n";
        }
        mDistanceMarker->setText(stream.str().c_str());
        auto pos = getScreenPosition( flightline.getMiddle() );
        pos.Y -= 10;
        mDistanceMarker->setRelativePosition( pos );

        video::SMaterial mat;
        mat.Lighting = false;
        driver->setMaterial(mat);
        driver->setTransform(video::ETS_WORLD, irr::core::matrix4());

        if(mMode == MOVE) {
            driver->draw3DLine(sp, aim);
        } else
        {
            driver->draw3DLine(sp, aim, video::SColor(255, 255, 0, 0));
        }
        if(mMode == MOVE) {
            driver->draw3DLine(aimbase + vector3df(-2, 0, -2), aimbase + vector3df(2, 0, 2),
                               video::SColor(128, 255, 255, 255));
            driver->draw3DLine(aimbase + vector3df(-2, 0, 2), aimbase + vector3df(2, 0, -2),
                               video::SColor(128, 255, 255, 255));
            driver->draw3DLine(aimbase + vector3df(0, -1, 0), aimbase + vector3df(0, 1, 0),
                               video::SColor(128, 255, 255, 255));
        }
    }
}

void ui::UnitCommand::onMouseMove(ui::IInputMode::ray_t ray)
{
    auto t = pick(state(), ray);
    if(t)
    {
        mCurrentAim = t->position();
        mMode = ATTACK;
        mCurrentAimShip = t->id();
    } else {
        mCurrentAim = aim(ray);
        mMode = MOVE;
    }
}

boost::optional<length_vec> ui::UnitCommand::aim(const ray_t &ray) const
{
    auto dir = ray.getVector();
    float tty = (mBaseY.value-ray.start.Y) / dir.Y;
    if (tty > 0) {
        auto target = ray.start + tty * dir;
        target /= 20; // to world coordinates
        return boost::optional<length_vec>(length_vec{target.X, mTargetY, target.Z});
    } else
    {
        return boost::none;
    }
}

void ui::UnitCommand::onWheel(float scroll)
{
    mTargetY += .1_km * (double)scroll;
    if(mCurrentAim)
        mCurrentAim.get().y = mTargetY;
}

void ui::UnitCommand::onKeyPress(irr::EKEY_CODE key)
{
    if(mActiveShipID != 0) {
        if (key == irr::KEY_KEY_1) {
            addCommand( cmd::SetWpnMode(mActiveShipID, 0, 0) );
        } else if (key == irr::KEY_KEY_2) {
            addCommand( cmd::SetWpnMode(mActiveShipID, 0, 1) );
        } else if (key == irr::KEY_KEY_3) {
            addCommand( cmd::SetWpnMode(mActiveShipID, 0, 2) );
        }
    }
}
