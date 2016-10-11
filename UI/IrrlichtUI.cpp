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
#include <sstream>
#include <iomanip>
#include <UI/inputs/UnitSelection.h>
#include "IInputMode.h"
#include "UI/inputs/UnitCommand.h"
#include <iostream>
#include "convert.h"

using namespace irr;
using spatacs::ui::IrrlichtUI;
using namespace spatacs;
using namespace ui;
namespace icore = irr::core;


const spatacs::core::Starship* pick(const spatacs::core::GameState& world, icore::line3df ray)
{
    const spatacs::core::Starship* picked = nullptr;
    f64 md = 1e10;
    for(auto& ship : world.getShips())
    {
        auto sp   = convert(ship.position());
        float rad = convert(ship.radius() + 50.0_m);
        f64 temp;
        if(ray.getIntersectionWithSphere(sp, rad, temp) && temp < md)
        {
            md = temp;
            picked = &ship;
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

    icore::line3df getPickLine() const
    {
        return mPickLine;
    }
private:
    bool handleMouseEvent( const SEvent::SMouseInput& mouse )
    {
        mPickLine = getMouseRay(mouse);
        switch(mouse.Event)
        {
            case EMIE_LMOUSE_PRESSED_DOWN:
                if(mInputMode)
                    mInputMode->onLeftClick(mPickLine);
                break;
            case EMIE_RMOUSE_PRESSED_DOWN:
                if(mInputMode)
                    mInputMode->onRightClick(mPickLine);
                break;
            case EMIE_MOUSE_MOVED:
                if(mInputMode)
                    mInputMode->onMouseMove(mPickLine);
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
    mDevice = createDevice(video::EDT_OPENGL, icore::dimension2du(800, 600), 32);
    mEventReceiver.reset( new EventRec );
    mEventReceiver->setCollisionManager( mDevice->getSceneManager()->getSceneCollisionManager() );
    mEventReceiver->mInputMode = std::make_shared<UnitSelection>(mOwnTeam);
    mEventReceiver->mInputMode->setMainUI(this);
    mDevice->setEventReceiver( mEventReceiver.get() );
    auto cam = mDevice->getSceneManager()->addCameraSceneNode();
    cam->setPosition({-20, 35, 0});
    cam->setTarget({50,0,0});
    mDevice->getSceneManager()->getRootSceneNode()->addChild( new scene::GridNode() );

    mMap = mDevice->getSceneManager()->addEmptySceneNode();
}

std::vector<spatacs::cmd::Command> IrrlichtUI::getCommands() const
{
    return mCommands.getCommands();
}

void IrrlichtUI::setState(const spatacs::core::GameState& state)
{
    auto smgr = mDevice->getSceneManager();
    mState = core::GameState(state);
    mCommands.validate(mState);

    // update the location map
    mMap->removeAll();
    for(auto& ship : mState.getShips()) {
        if(!ship.alive())
            continue;
        auto node = new scene::ShipFx( mMap, mDevice->getSceneManager() );
        video::SColor colors[] = {{255, 0, 200, 0}, {255, 200, 0, 0}};
        node->setColor( colors[ship.team()-1] );
        auto ss = ship.shield_strength();
        node->setShieldStatus( ss.current / ss.max );

        drop_ptr<scene::ISceneNodeAnimator> ani{smgr->createFlyStraightAnimator(convert(ship.position()),
                                          convert(ship.position() + 1.0_s * ship.velocity()), 1000)};
        node->addAnimator(ani.get());
    }

    for(auto& proj : mState.getProjectiles())
    {
        auto shotfx = new scene::ShotFx( mMap, mDevice->getSceneManager() );
        shotfx->setShot(convert(proj.velocity()*1.0_s));
        drop_ptr<scene::ISceneNodeAnimator> ani{smgr->createFlyStraightAnimator(convert(proj.position()),
                                          convert(proj.position() + 1.0_s * proj.velocity()), 1000)};
        shotfx->addAnimator(ani.get());
    }

}

bool IrrlichtUI::step()
{
    if(mDevice->run()) {
        mDevice->getVideoDriver()->beginScene();
        mDevice->getSceneManager()->drawAll();
        mDevice->getGUIEnvironment()->drawAll();
        try {
            if(mEventReceiver->mInputMode)
               mEventReceiver->mInputMode->draw(mDevice->getVideoDriver());
        } catch( std::exception& ex )
        {
        }
        mDevice->getVideoDriver()->endScene();
        return true;
    }
    exit(0); /// \todo not like this ... not like this
    return false;
}

void IrrlichtUI::notifyEvents(const std::vector<std::unique_ptr<spatacs::events::IEvent>>& events)
{
    using drop_ani_ptr = drop_ptr<scene::ISceneNodeAnimator>;
    auto smgr = mDevice->getSceneManager();
    for(auto& evt : events)
    {
        try {
            if (auto h = dynamic_cast<const events::Damage*>(evt.get())) {
                uint64_t ship = h->id();
                float dmg = h->damage().high_explosive + h->damage().kinetic + h->damage().armour_piercing
                            + h->damage().shield_overload;
                if (dmg > 0.1) {
                    float s = 5 * dmg + 1;
                    auto pos = mState.getShip(ship).position();
                    auto bb = smgr->addBillboardSceneNode();
                    bb->setPosition(convert(pos));
                    bb->setMaterialFlag(video::EMF_LIGHTING, false);
                    bb->setSize(s, s, s);
                    bb->setMaterialType(video::EMT_TRANSPARENT_ALPHA_CHANNEL);
                    bb->setMaterialTexture(0, mDevice->getVideoDriver()->getTexture("data/attack.png"));
                    drop_ani_ptr a{smgr->createDeleteAnimator(200)};
                    bb->addAnimator(a.get());
                }
            } else if (auto h = dynamic_cast<const events::HitShield*>(evt.get())) {
                auto& ship = mState.getShip(h->id());
                if(ship.shield_strength().current > 0) {
                    float s = 5;
                    auto pos = ship.position();
                    auto bb = smgr->addBillboardSceneNode();
                    bb->setPosition(convert(pos));
                    bb->setMaterialFlag(video::EMF_LIGHTING, false);
                    bb->setSize(s, s, s);
                    bb->setMaterialType(video::EMT_TRANSPARENT_ALPHA_CHANNEL);
                    bb->setRotation( irr::core::vector3df(rand() % 100, rand() % 100, rand() % 100) );
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
                                                            convert(pos + ship.velocity() * 1.0_s),
                                                            1000)
                    );
                    bb->addAnimator(a.get());
                }
            }


        } catch( std::exception& e )
        {
            std::cerr << e.what() << "\n";
        }
    }
}

struct GetShip : public boost::static_visitor<std::uint64_t>
{
    template<class T>
    std::uint64_t operator()(const T& t) const
    {
        return t.object();
    }
};


void IrrlichtUI::addCommand( spatacs::cmd::Command c )
{
    if(mState.getShip(get_ship(c)).team() == mOwnTeam)
        mCommands.addCommand(std::move(c));
}

IrrlichtUI::~IrrlichtUI()
{

}

IrrlichtUI::IrrlichtUI(std::uint64_t team) : mOwnTeam(team)
{
}
