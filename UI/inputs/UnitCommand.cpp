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
#include "game/GameState.h"
#include <irrlicht/ICameraSceneNode.h>
#include <iostream>
#include "UI/convert.h"
#include "game/Starship.h"
#include "UI/IrrlichtUI.h"
#include "cmd/CommandManager.h"
#include <irrlicht/ISceneManager.h>
#include "UI/gfx/MultiLineNode.h"
#include "UI/panels/ShipStatus.h"
#include <irrlicht/IBillboardSceneNode.h>
#include "UI/panels/DamageReport.h"
#include "UI/panels/WeaponsPanel.h"
#include "game/systems/Ammunition.h"
#include "game/SubSystems.h"

using namespace spatacs;

/// \todo this is copy paste, improve that!
const game::Starship* pick(const game::State& world, irr::core::line3df ray);

ui::UnitCommand::UnitCommand(game::ObjectID id) :
        mActiveShipID(id)
{

}

void ui::UnitCommand::init(irr::gui::IGUIEnvironment* guienv, irr::scene::ISceneManager* smgr)
{
    mTargetY = state().getObject(mActiveShipID).position().y;
    auto txt = guienv->addStaticText(L"", irr::core::recti(0, 0, 100, 40));
    txt->setOverrideColor( irr::video::SColor(255, 128, 128, 255) );
    mDistanceMarker.reset(txt);

    mShipStatus.reset(new ShipStatus(guienv, guienv->getRootGUIElement(), -1,
                                                 irr::core::recti(10, 10, 100, 170)));

    mTargetStatus.reset( new ShipStatus(guienv, guienv->getRootGUIElement(), -1,
            irr::core::recti(700, 10, 790, 90)));
    mTargetStatus->setShowFuel(false);
    mTargetStatus->setShowAmmunition(false);
    mTargetStatus->setShowPower(false);

    txt = guienv->addStaticText(L"", irr::core::recti(10, 175, 100, 195));
    txt->setOverrideColor( irr::video::SColor(255, 128, 128, 255) );
    mSpeedInfo.reset(txt);

    mTrajectoryPlotter.reset( new irr::scene::MultiLineNode(smgr->getRootSceneNode(), smgr) );

    mDamageReport.reset(new ui::DamageReport(guienv, guienv->getRootGUIElement(), -1, irr::core::recti(10, 500, 110, 600)));
    mWeaponsPanel.reset(new ui::WeaponsPanel(guienv, guienv->getRootGUIElement(), -1, irr::core::recti(110, 10, 300, 50)));
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
            if(event.Shift) {
                getCmdMgr().addCommand(mActiveShipID,
                                       cmd::Move(std::make_unique<cmd::movement::EngageTarget>(t->id(), mTargetSpeed)));

            } else {
                getCmdMgr().addCommand(mActiveShipID, cmd::Attack(t->id()));
            }
        }
    } else {
        auto target = aim(ray);
        if(target) {
            auto vec = target.get();
            bool add_cmd = true;
            if(event.Shift) {
                auto& cmd = getCmdMgr().getCommandsOf(mActiveShipID);
                if(cmd.move) {
                    auto c = &cmd.move.getCommand();
                    if (dynamic_cast<const cmd::movement::FollowRoute*>(c)) {
                        auto fr = std::make_unique<cmd::movement::FollowRoute>(
                                dynamic_cast<const cmd::movement::FollowRoute&>(*c));
                        fr->addWaypoint(vec);
                        fr->setSpeed(mTargetSpeed);
                        getCmdMgr().addCommand(mActiveShipID,
                                               cmd::Move(std::move(fr)));
                        add_cmd = false;
                    }
                }
            }
            if(add_cmd)
            {
                getCmdMgr().addCommand(mActiveShipID,
                                       cmd::Move(std::make_unique<cmd::movement::FollowRoute>(vec, mTargetSpeed)));
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
        std::size_t ammo_id = std::size_t(-1);
        if (key == irr::KEY_KEY_1) {
            ammo_id = 0;
        } else if (key == irr::KEY_KEY_2) {
            ammo_id = 1;
        } else if (key == irr::KEY_KEY_3) {
            ammo_id = 2;
        }

        game::systems::ListAmmunition la;
        ship.components().apply(la);

        if(ammo_id != -1 && ammo_id < la.getAmmos().size())
        {
            auto wpn_id_range = ship.components().get_matching_ids(core::type_v<game::systems::signatures::AimSignature>);
            for(const auto& id : wpn_id_range) {
                getCmdMgr().addCommand(mActiveShipID,
                                       cmd::SetWpnMode(mActiveShipID, id, la.getAmmos().at(ammo_id).data.name));
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

        double change_sgen_activity = 0.0;
        if( key == irr::KEY_KEY_O )
        {
            change_sgen_activity = 0.1;
        } else if( key == irr::KEY_KEY_L )
        {
            change_sgen_activity = -0.1;
        }

        // a lambda that iterates over all components with given signature and
        // creates a SetSystemActivity command.
        auto apply_change = [&](auto type, double change)
        {
            using namespace boost::adaptors;

            auto transform = [&](auto id) {
                return cmd::SetSystemActivity(
                        mActiveShipID,
                        id,
                        ship.components().get_component<game::Activity>(id).get() + change
                );
            };

            auto id_range = ship.components().get_matching_ids(type);

            for(auto&& cmd : id_range | transformed(transform))
            {
                getCmdMgr().addCommand(mActiveShipID, std::move(cmd));
            }
        };

        if(change_sgen_activity != 0)
        {
            using ShieldSig = core::ecs::Signature<const game::ShieldGeneratorData, const game::Activity>;
            apply_change(core::type_v<ShieldSig>, change_sgen_activity);
        }

        double change_pp_activity = 0.0;
        if( key == irr::KEY_KEY_I )
        {
            change_pp_activity = 0.1;
        } else if( key == irr::KEY_KEY_K )
        {
            change_pp_activity = -0.1;
        }

        if(change_pp_activity != 0)
        {
            using PPSig = core::ecs::Signature<const game::PowerPlantData, const game::Activity>;
            apply_change(core::type_v<PPSig>, change_pp_activity);
        }

        double change_eg_activity = 0.0;
        if( key == irr::KEY_KEY_U )
        {
            change_eg_activity = 0.1;
        } else if( key == irr::KEY_KEY_J )
        {
            change_eg_activity = -0.1;
        }

        if(change_eg_activity != 0)
        {
            using EGSig = core::ecs::Signature<const game::EngineData, const game::Activity>;
            apply_change(core::type_v<EGSig>, change_eg_activity);
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
    mWeaponsPanel->update(ship);

    game::ObjectID enemy_ship{};

    auto& cmd = getCmdMgr().getCommandsOf(mActiveShipID);
    auto& mv = cmd.move;
    auto src = sp;
    for(unsigned i = 0; i < mv.point_count(); ++i) {
        mTrajectoryPlotter->addLine(src, convert(mv.target(i)), video::SColor(255, 0, 128, 0));
        src = convert(mv.target(i));
    }
    if(cmd.attack)
    {
        auto& at = cmd.attack.get();
        if(state().hasObject(at.target())) {
            enemy_ship = at.target();
            mTrajectoryPlotter->addLine(sp, convert(state().getObject(at.target()).position()),
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
            mTrajectoryPlotter->addLine(aimbase + vector3df(0, -0.5f, 0), aimbase + vector3df(0, 0.5, 0),
                                        video::SColor(128, 255, 255, 255));
        }
    }

}
