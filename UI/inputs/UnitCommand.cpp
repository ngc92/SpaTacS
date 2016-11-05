//
// Created by erik on 9/22/16.
//

#include "cmd/Attack.h"
#include "cmd/Move.h"
#include <irrlicht/IVideoDriver.h>
#include <irrlicht/IGUIEnvironment.h>
#include <irrlicht/IGUIStaticText.h>
#include <sstream>
#include <iomanip>
#include "UnitCommand.h"
#include "core/GameState.h"
#include <irrlicht/ICameraSceneNode.h>
#include <iostream>
#include "UI/gfx/ShipStatusUI.h"
#include "UI/convert.h"
#include "game/Starship.h"
#include "UI/IrrlichtUI.h"
#include "cmd/CommandManager.h"
#include <irrlicht/ISceneManager.h>
#include "game/systems.h"
#include "UI/gfx/MultiLineNode.h"
#include "ShipStatus.h"
#include <irrlicht/IBillboardSceneNode.h>
#include "UI/panels/DamageReport.h"

using namespace spatacs;

/// \todo this is copy paste, improve that!
const game::Starship* pick(const core::GameState& world, irr::core::line3df ray);

ui::UnitCommand::UnitCommand(game::ObjectID id) :
        mActiveShipID(id)
{

}

void ui::UnitCommand::init(irr::gui::IGUIEnvironment* guienv, irr::scene::ISceneManager* smgr)
{
    mTargetY = state().getShip(mActiveShipID).position().y;
    auto txt = guienv->addStaticText(L"", irr::core::recti(0, 0, 100, 40));
    txt->setOverrideColor( irr::video::SColor(255, 128, 128, 255) );
    mDistanceMarker.reset(txt);

    auto sstat = new irr::gui::ShipStatusUI(guienv, guienv->getRootGUIElement(), -1,
                                            irr::core::recti(10, 10, 100, 170));
    mShipStatus = std::make_unique<ShipStatusUI>(sstat);

    mShipStatus->addWeaponStatus( guienv->addStaticText(L"", irr::core::recti(110, 10, 200, 50)) );
    mShipStatus->addWeaponStatus( guienv->addStaticText(L"", irr::core::recti(210, 10, 300, 50)) );

    sstat = new irr::gui::ShipStatusUI(guienv, guienv->getRootGUIElement(), -1,
                                       irr::core::recti(700, 10, 790, 90));
    mTargetStatus = std::make_unique<ShipStatusUI>(sstat);
    mTargetStatus->setShowSystemHealth(false);
    mTargetStatus->setShowFuel(false);
    mTargetStatus->setShowAmmunition(false);
    mTargetStatus->setShowWeapons(false);
    mTargetStatus->setShowPower(false);

    txt = guienv->addStaticText(L"", irr::core::recti(10, 175, 100, 195));
    txt->setOverrideColor( irr::video::SColor(255, 128, 128, 255) );
    mSpeedInfo.reset(txt);

    mTrajectoryPlotter.reset( new irr::scene::MultiLineNode(smgr->getRootSceneNode(), smgr) );

    mDamageReport.reset(new ui::DamageReport(guienv, guienv->getRootGUIElement(), -1, irr::core::recti(10, 500, 110, 600)));
}


ui::UnitCommand::~UnitCommand()
{
}

void ui::UnitCommand::onRightClick(ray_t ray, const irr::SEvent::SMouseInput& event)
{
    auto& ship = state().getShip(mActiveShipID);
    auto t = pick(state(), ray);
    if( t != 0 )
    {
        if(t->team() != ship.team())
        {
            getCmdMgr().addCommand(mActiveShipID, cmd::Attack(t->id()));
        }
    } else {
        auto target = aim(ray);
        if(target) {
            auto vec = target.get();
            if(event.Shift) {
                auto& cmd = getCmdMgr().getCommandsOf(mActiveShipID);
                cmd::Move cp = cmd.move;
                cp.addWaypoint(vec);
                cp.setSpeed( mTargetSpeed );
                getCmdMgr().addCommand(mActiveShipID, cp);
            } else {
                getCmdMgr().addCommand(mActiveShipID, cmd::Move(vec, mTargetSpeed));
            }
        }
    }
}

void ui::UnitCommand::onMouseMove(ui::IInputMode::ray_t ray, const irr::SEvent::SMouseInput& event)
{
    auto t = pick(state(), ray);
    if(t && t->team() != state().getShip(mActiveShipID).team())
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
    float tty = (mBaseY-ray.start.Y) / dir.Y;
    if (tty > 0) {
        auto target = ray.start + tty * dir;
        target.Y = convert(mTargetY);
        return convert(target);
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
    if(mActiveShipID)
    {
        const auto& ship = state().getShip(mActiveShipID);
        for(unsigned i = 0; i < ship.weapon_count(); ++i) {
            std::size_t ammo_id = std::size_t(-1);
            if (key == irr::KEY_KEY_1) {
                ammo_id = 0;
            } else if (key == irr::KEY_KEY_2) {
                ammo_id = 1;
            } else if (key == irr::KEY_KEY_3) {
                ammo_id = 2;
            }

            if(ammo_id != -1)
            {
                game::ListAmmunition la;
                ship.components().apply(la);
                if(ammo_id < la.getAmmos().size()) {
                    getCmdMgr().addCommand(mActiveShipID,
                                           cmd::SetWpnMode(mActiveShipID, i, la.getAmmos().at(ammo_id).data.name));
                }
            }

        }

        if( key == irr::KEY_PLUS )
        {
            mTargetSpeed += 0.05_kps;
        } else if( key == irr::KEY_MINUS )
        {
            mTargetSpeed -= 0.05_kps;
            if(mTargetSpeed < 0.1_kps)
                mTargetSpeed = 0.1_kps;
        }
    }
}

void ui::UnitCommand::step()
{
    mBaseY = std::round(getCamera()->getTarget().Y / 10.f) * 10.f;

    using namespace irr;

    mTrajectoryPlotter->clear();

    if(!mActiveShipID)
        return;

    // ship info text
    auto& ship = state().getShip(mActiveShipID);
    if(!ship.alive()) {
        mActiveShipID.reset();
        return;
    }
    auto sp = convert(ship.position());
    mShipStatus->update(ship);
    mDamageReport->update(ship);

    game::ObjectID enemy_ship{};

    auto& cmd = getCmdMgr().getCommandsOf(mActiveShipID);
    auto& mv = cmd.move;
    auto src = sp;
    for(unsigned i = 0; i < mv.waypoint_count(); ++i) {
        mTrajectoryPlotter->addLine(src, convert(mv.target(i)), video::SColor(255, 0, 128, 0));
        src = convert(mv.target(i));
    }
    if(cmd.attack)
    {
        auto& at = cmd.attack.get();
        if(state().hasObject(at.target())) {
            enemy_ship = at.target();
            mTrajectoryPlotter->addLine(sp, convert(state().getShip(at.target()).position()),
                                        video::SColor(255, 128, 0, 0));
        } else
        {
            std::cerr << "Command manager kept invalid command!\n";
        }
    }


    if(mMode == MOVE)
    {
        mTargetStatus->setVisible(false);
    } else
    {
        enemy_ship = mCurrentAimShip;
    }

    if(state().hasObject(enemy_ship)) {
        auto& target = state().getShip(enemy_ship);
        mTargetStatus->setVisible(true);
        mTargetStatus->update(target);
    }

    {
        std::wstringstream stream;
        stream << std::fixed << std::setprecision(1);
        stream << "speed:  " << length(ship.velocity()) << "\n";
        stream << "target: " << mTargetSpeed << "\n";
        mSpeedInfo->setText(stream.str().c_str());
    }

    if (mCurrentAim) {
        auto aim = convert(mCurrentAim.get());
        auto aimbase = aim;
        aimbase.Y = std::round(getCamera()->getTarget().Y / 10) * 10;
        irr::core::line3df flightline(sp, aim);

        std::wstringstream stream;
        stream << std::fixed << std::setprecision(1) << length(ship.position() - mCurrentAim.get()) << "\n";
        mDistanceMarker->setText(stream.str().c_str());
        auto pos = getScreenPosition( flightline.getMiddle() );
        pos.Y -= 10;
        mDistanceMarker->setRelativePosition( pos );

        if(mMode == MOVE) {
            mTrajectoryPlotter->addLine(sp, aim, video::SColor(255, 255, 255, 255));
        } else
        {
            mTrajectoryPlotter->addLine(sp, aim, video::SColor(255, 255, 0, 0));
        }
        if(mMode == MOVE) {
            using irr::core::vector3df;
            mTrajectoryPlotter->addLine(aimbase + vector3df(-1, 0, -1), aimbase + vector3df(1, 0, 1),
                                        video::SColor(128, 255, 255, 255));
            mTrajectoryPlotter->addLine(aimbase + vector3df(-1, 0, 1), aimbase + vector3df(1, 0, -1),
                                        video::SColor(128, 255, 255, 255));
            mTrajectoryPlotter->addLine(aimbase + vector3df(0, -0.5, 0), aimbase + vector3df(0, 0.5, 0),
                                        video::SColor(128, 255, 255, 255));
        }
    }

}
