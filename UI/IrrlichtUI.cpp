//
// Created by erik on 9/20/16.
//

#include "IrrlichtUI.h"
#include <irrlicht/irrlicht.h>
#include <algorithm>
#include "gfx/GridNode.h"
#include "events/Combat.h"
#include "gfx/ShotFx.h"
#include "gfx/ShipFx.h"
#include "gfx/ShieldFx.h"
#include <sstream>
#include <iomanip>
#include <UI/inputs/UnitSelection.h>
#include "IInputMode.h"
#include "UI/inputs/UnitCommand.h"
#include <iostream>
#include "convert.h"
#include "game/Starship.h"
#include "game/Projectile.h"
#include "game/Shield.h"
#include "game/events/events.h"

using namespace irr;
using spatacs::ui::IrrlichtUI;
using namespace spatacs;
using namespace ui;
namespace icore = irr::core;


const game::Starship* pick(const spatacs::game::GameState& world, icore::line3df ray)
{
    const spatacs::game::Starship* picked = nullptr;
    f64 md = 1e10;
    for(auto& obj : world)
    {
        auto ship = dynamic_cast<const spatacs::game::Starship*>(&obj);
        if(!ship)
            continue;

        auto sp   = convert(ship->position());
        float rad = convert(ship->radius() + 100.0_m);
        f64 temp;
        if(ray.getIntersectionWithSphere(sp, rad, temp) && temp < md)
        {
            md = temp;
            picked = ship;
        }
    }
    return picked;
}


class IrrlichtUI::EventRec : public IEventReceiver
{
public:
    ~EventRec()
    {
    }

    void setCollisionManager(scene::ISceneCollisionManager* cm)
    {
        cm->grab();
        mCollisionManager.reset(cm);
    }

    virtual bool OnEvent(const SEvent& event) override
    {
        switch(event.EventType)
        {
            case EET_MOUSE_INPUT_EVENT:
                return handleMouseEvent( event.MouseInput );
            case EET_KEY_INPUT_EVENT:
                return handleKeyEvent( event.KeyInput );
        }
        return false;
    }

    bool mWantPause = false;
private:
    bool handleMouseEvent( const SEvent::SMouseInput& mouse )
    {
        mPickLine = getMouseRay(mouse);
        if(!mInputMode)
            return false;

        switch(mouse.Event)
        {
            case EMIE_LMOUSE_PRESSED_DOWN:
                mInputMode->onLeftClick(mPickLine, mouse);
                break;
            case EMIE_RMOUSE_PRESSED_DOWN:
                mInputMode->onRightClick(mPickLine, mouse);
                break;
            case EMIE_LMOUSE_LEFT_UP:
                mInputMode->onLeftMouseUp(mPickLine, mouse);
                break;
            case EMIE_MOUSE_MOVED:
                mInputMode->onMouseMove(mPickLine, mouse);
                break;
            case EMIE_MOUSE_WHEEL:
                mInputMode->onWheel(mouse.Wheel);
        }

        return false;
    }

    bool handleKeyEvent(const SEvent::SKeyInput& key)
    {
        if(key.PressedDown)
        {
            if(key.Key == KEY_KEY_P)
            {
                mWantPause = !mWantPause;
            }
            if(mInputMode)
                mInputMode->onKeyPress(key.Key);
        } else
        {
            if(mInputMode)
                mInputMode->onKeyRelease(key.Key);
        }
        return false;
    }

    icore::line3df getMouseRay(const SEvent::SMouseInput& mouse)
    {
        if(!mCollisionManager)
            return icore::line3df();
        icore::vector2di pos{ mouse.X, mouse.Y };
        return mCollisionManager->getRayFromScreenCoordinates(pos);
    }

    icore::line3df mPickLine;
    drop_ptr<scene::ISceneCollisionManager> mCollisionManager;

public:
    std::shared_ptr<IInputMode> mInputMode;
};

void IrrlichtUI::init()
{
    mState = std::make_shared<spatacs::game::GameState>();
    mEventReceiver.reset( new EventRec );
    mEventReceiver->setCollisionManager( mDevice->getSceneManager()->getSceneCollisionManager() );
    mEventReceiver->mInputMode = std::make_shared<UnitSelection>(mOwnTeam);
    mEventReceiver->mInputMode->setMainUI(this);
    auto cam = mDevice->getSceneManager()->addCameraSceneNode();
    cam->setPosition({-20, 35, 0});
    cam->setTarget({50,0,0});
    mDevice->getSceneManager()->getRootSceneNode()->addChild( new scene::GridNode() );

    mMap = mDevice->getSceneManager()->addEmptySceneNode();
}

void IrrlichtUI::setState(const state_t& bstate)
{
    auto state = std::dynamic_pointer_cast<const game::GameState>(bstate);

    auto smgr = mDevice->getSceneManager();
    mState = state;

    // update the location map
    mMap->removeAll();
    for(auto& obj : *mState) {
        if(!obj.alive())
            continue;
        irr::scene::ISceneNode* node = nullptr;
        if(auto ship = dynamic_cast<const game::Starship*>(&obj))
        {
            auto shipfx = new scene::ShipFx(mMap, mDevice->getSceneManager());
            video::SColor colors[] = {{255, 0,   200, 0},
                                      {255, 200, 0,   0}};
            shipfx->setColor(colors[ship->team() - 1]);
            shipfx->setHullStatus(ship->armour() / ship->max_armour());
            node = shipfx;
        }
         else if(auto proj = dynamic_cast<const game::Projectile*>(&obj))
        {
            auto shotfx = new scene::ShotFx( mMap, mDevice->getSceneManager() );
            shotfx->setShot(convert(obj.velocity()*1.0_s));

            if(proj->damage().shield_overload > 0)
            {
                shotfx->setColor(video::SColor(255, 255, 0, 128));
            } else if(proj->damage().high_explosive > 0)
            {
                shotfx->setColor(video::SColor(255, 255, 128, 0));
            }
            node = shotfx;
        } else if(auto shield = dynamic_cast<const game::Shield*>(&obj))
        {
            auto fx = new scene::ShieldFx(mMap, mDevice->getSceneManager());
            fx->setColor(video::SColor(128, 32, 32, 200));
            fx->setStatus(shield->current() / shield->maximum());
            node = fx;
        }

        if(node) {
            drop_ptr<scene::ISceneNodeAnimator> ani{smgr->createFlyStraightAnimator(convert(obj.position()),
                                                                                    convert(obj.position() +
                                                                                            0.1_s * obj.velocity()),
                                                                                    100)};
            node->addAnimator(ani.get());
        }
    }

}

bool IrrlichtUI::step()
{
    mEventReceiver->mInputMode->step();
    try {
        if(mEventReceiver->mInputMode) {
            mEventReceiver->mInputMode->step();
        }
    } catch( std::exception& ex )
    {
    }
    return true;
}

struct IrrlichtUI::Dispatch : public boost::static_visitor<void>
{
    Dispatch(IrrlichtUI* iui) : ui(iui) {}
    IrrlichtUI* ui;
    template<class T>
    void operator()(T&& e)
    {
        ui->onEvent(std::forward<T>(e));
    }
};

void IrrlichtUI::notify(const notify_t& events)
{
    IrrlichtUI::Dispatch dispatch{this};
    for(auto& evt : events.get_as<game::events::notification_t>())
    {
        boost::apply_visitor(dispatch, evt);
    }
}

using drop_ani_ptr = drop_ptr<scene::ISceneNodeAnimator>;

void IrrlichtUI::onEvent(const game::events::ReceiveDamage& ev)
{
    auto smgr = mDevice->getSceneManager();
    if (ev.damage > 0.1)
    {
        float s = 5 * ev.damage + 1;
        auto pos = mState->getObject(ev.id).position();
        auto bb = smgr->addBillboardSceneNode();
        bb->setPosition(convert(pos));
        bb->setMaterialFlag(video::EMF_LIGHTING, false);
        bb->setSize(s, s, s);
        bb->setMaterialType(video::EMT_TRANSPARENT_ALPHA_CHANNEL);
        bb->setMaterialTexture(0, mDevice->getVideoDriver()->getTexture("data/attack.png"));
        drop_ani_ptr a{smgr->createDeleteAnimator(200)};
        bb->addAnimator(a.get());
    }
}

void IrrlichtUI::onEvent(const game::events::ShieldAbsorbtion& ev)
{
    auto smgr = mDevice->getSceneManager();
    const auto& shield = dynamic_cast<const game::Shield&>(mState->getObject(ev.id));
    if(shield.current() > 0) {
        float s = 5;
        auto pos = shield.position();
        auto bb = smgr->addBillboardSceneNode();
        bb->setPosition(convert(pos));
        bb->setMaterialFlag(video::EMF_LIGHTING, false);
        bb->setSize(s, s, s);
        bb->setMaterialType(video::EMT_TRANSPARENT_ALPHA_CHANNEL);
        drop_ani_ptr a{smgr->createDeleteAnimator(500)};
        bb->addAnimator(a.get());
        irr::core::array<irr::video::ITexture*> textures;
        for (int i = 1; i <= 32; ++i) {
            textures.push_back(mDevice->getVideoDriver()->getTexture(
                    ("data/fx7_energyBall/aura_test_1_32_" + std::to_string(i) + ".png").c_str()));
        }
        a.reset(smgr->createTextureAnimator(textures, 20));
        bb->addAnimator(a.get());
        a.reset(smgr->createFlyStraightAnimator(convert(pos),
                                                convert(pos + shield.velocity() * 1.0_s),
                                                1000)
        );
        bb->addAnimator(a.get());
    }
}

cmd::CommandManager& IrrlichtUI::getCommandMgr()
{
    return *mCommands;
}

IrrlichtUI::~IrrlichtUI()
{

}

IrrlichtUI::IrrlichtUI(std::uint64_t team, irr::IrrlichtDevice* device, std::shared_ptr<cmd::CommandManager> cmd) :
        mOwnTeam(team),
        mDevice(device),
        mCommands( std::move(cmd) )
{
}

void IrrlichtUI::getCommandEvents(std::vector<events::EventPtr>& evts)
{
}

bool IrrlichtUI::pause() const
{
    return mEventReceiver->mWantPause;
}

bool IrrlichtUI::handleUIEvent(const irr::SEvent& ev)
{
    return mEventReceiver->OnEvent(ev);
}
