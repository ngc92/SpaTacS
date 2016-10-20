//
// Created by erik on 9/22/16.
//

#include "UI/cmd/Attack.h"
#include "UI/cmd/Move.h"
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
#include "UI/cmd/CommandManager.h"
#include <irrlicht/ISceneManager.h>
#include "game/systems.h"
#include "UI/gfx/MultiLineNode.h"

using namespace spatacs;

/// \todo this is copy past, improve that!
const game::Starship* pick(const core::GameState& world, irr::core::line3df ray);

ui::UnitCommand::UnitCommand(std::uint64_t id) :
        mActiveShipID(id)
{

}

void ui::UnitCommand::init(irr::gui::IGUIEnvironment* guienv, irr::scene::ISceneManager* smgr)
{
    mTargetY = state().getShip(mActiveShipID).position().y;
    auto txt = guienv->addStaticText(L"", irr::core::recti(0, 0, 100, 40));
    txt->setOverrideColor( irr::video::SColor(255, 128, 128, 255) );
    mDistanceMarker.reset(txt);

    auto sstat = new irr::gui::ShipStatusUI(guienv, guienv->getRootGUIElement(), -1, irr::core::recti(10, 10, 100, 150));
    mShipInfo.reset(sstat);

    sstat = new irr::gui::ShipStatusUI(guienv, guienv->getRootGUIElement(), -1, irr::core::recti(700, 10, 790, 90));
    mTargetInfo.reset(sstat);

    txt = guienv->addStaticText(L"", irr::core::recti(10, 155, 100, 175));
    txt->setOverrideColor( irr::video::SColor(255, 128, 128, 255) );
    mSpeedInfo.reset(txt);

    mTrajectoryPlotter.reset( new irr::scene::MultiLineNode(smgr->getRootSceneNode(), smgr) );
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
    }else {
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

void ui::UnitCommand::onMouseMove(ui::IInputMode::ray_t ray)
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
    if(mActiveShipID != 0) {
        if (key == irr::KEY_KEY_1) {
            getCmdMgr().addCommand(mActiveShipID, cmd::SetWpnMode(mActiveShipID, 0, "AP") );
        } else if (key == irr::KEY_KEY_2) {
            getCmdMgr().addCommand(mActiveShipID, cmd::SetWpnMode(mActiveShipID, 0, "HE") );
        } else if (key == irr::KEY_KEY_3) {
            getCmdMgr().addCommand(mActiveShipID, cmd::SetWpnMode(mActiveShipID, 0, "SO") );
        } else if( key == irr::KEY_PLUS )
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

namespace
{

    class SysInfo : public core::System<const game::ComponentEntity, SysInfo, core::Signature<game::Health, game::Name>>
    {
    public:
        SysInfo(irr::gui::ShipStatusUI* ui) : mUI(ui)
        {
        }

        void apply(const game::Health& h, const game::Name& n)
        {
            irr::gui::SystemStatus status{n.name, h.current, h.maximum};
            mUI->pushSystem( status );

        }
    private:
        irr::gui::ShipStatusUI* mUI;
    };
}

void ui::UnitCommand::step()
{
    mBaseY = std::round(getCamera()->getTarget().Y / 10.f) * 10.f;

    using namespace irr;

    mTrajectoryPlotter->clear();

    if(mActiveShipID == 0)
        return;

    // ship info text
    auto& ship = state().getShip(mActiveShipID);
    if(!ship.alive()) {
        mActiveShipID = 0;
        return;
    }
    auto sp = convert(ship.position());
    {
        mShipInfo->setShipName( ship.name() );
        mShipInfo->clearSystems();
        SysInfo si(mShipInfo.get());
        game::TankInfo ti;
        ship.components().apply(si);
        ship.components().apply(ti);
        game::ListAmmunition la;
        ship.components().apply(la);
        mShipInfo->pushSystem( irr::gui::SystemStatus{"shield",  ship.shield(), ship.max_shield()} );
        mShipInfo->pushSystem( irr::gui::SystemStatus{"hull",  ship.hull_status().current, ship.hull_status().max} );
        mShipInfo->pushSystem( irr::gui::SystemStatus{"structure", ship.hp(), ship.max_hp()} );
        mShipInfo->pushSystem( irr::gui::SystemStatus{"fuel", ti.fuel() / 1.0_t, ti.capacity() / 1.0_t} );
        std::size_t acount = 0;
        for(auto& a : la)
            acount += a.second;
        mShipInfo->pushSystem( irr::gui::SystemStatus{"ammo", acount, la.capacity()} );
        /// \todo power plant
    }

    if(mMode == MOVE)
    {
        mTargetInfo->setVisible(false);
    } else
    {
        if(state().hasObject(mCurrentAimShip)) {
            auto& target = state().getShip(mCurrentAimShip);
            mTargetInfo->setVisible(true);
            mTargetInfo->setShipName(target.name());
            mTargetInfo->clearSystems();
            mTargetInfo->pushSystem(irr::gui::SystemStatus{"shield", target.shield(), target.max_shield()});
            mTargetInfo->pushSystem(
                    irr::gui::SystemStatus{"hull", target.hull_status().current, target.hull_status().max});
            mTargetInfo->pushSystem(irr::gui::SystemStatus{"structure", target.hp(), target.max_hp()});
        }
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
            mTrajectoryPlotter->addLine(sp, convert(state().getShip(at.target()).position()),
                                        video::SColor(255, 128, 0, 0));
        } else
        {
            std::cerr << "Command manager kept invalid command!\n";
        }
    }


}
