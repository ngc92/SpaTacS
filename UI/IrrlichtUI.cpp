//
// Created by erik on 9/20/16.
//

#include "IrrlichtUI.h"
#include <irrlicht/irrlicht.h>
#include <algorithm>
#include "gfx/GridNode.h"
#include "gfx/LocationPlotter3D.h"
#include "events/Combat.h"
#include "gfx/ShotFx.h"
#include <sstream>
#include <iomanip>
#include <functional>
#include <UI/inputs/UnitSelection.h>
#include "IInputMode.h"
#include "UI/inputs/UnitCommand.h"
#include <iostream>

using namespace irr;
using spatacs::ui::IrrlichtUI;
using namespace spatacs;
namespace icore = irr::core;


const spatacs::core::Starship* pick(const spatacs::core::GameState& world, icore::line3df ray)
{
    const spatacs::core::Starship* picked = nullptr;
    f64 md = 1e10;
    for(auto& ship : world.getShips())
    {
        icore::vector3df sp{ ship.position().x.value, ship.position().y.value, ship.position().z.value };
        sp *= 20;
        float rad = (ship.radius() + 50.0_m).value * 20;
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
        if(mCollisionManager)
            mCollisionManager->drop();
    }

    void setCollisionManager(scene::ISceneCollisionManager* cm)
    {
        if(mCollisionManager)
            mCollisionManager->drop();
        mCollisionManager = cm;
        cm->grab();
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
    scene::ISceneCollisionManager* mCollisionManager = nullptr;

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

    mShipPlot = new scene::LocationPlotter3D();
    mShipPlot->setColor(1, {255, 0, 200, 0});
    mShipPlot->setColor(2, {255, 200, 0, 0});
    mDevice->getSceneManager()->getRootSceneNode()->addChild(mShipPlot);
}

std::vector<spatacs::cmd::Command> IrrlichtUI::getCommands() const
{
    return mCommands.getCommands();
}

void IrrlichtUI::setState(const spatacs::core::GameState& state)
{
    mState = core::GameState(state);
    mCommands.validate(mState);

    // update the location map
    std::vector<irr::scene::Location> ps;
    for(auto& ship : mState.getShips()) {
        if(!ship.alive())
            continue;
        icore::vector3df pos{ship.position().x.value, ship.position().y.value, ship.position().z.value};
        pos *= 20;
        icore::vector3df vel{ship.velocity().x.value, ship.velocity().y.value, ship.velocity().z.value};
        vel *= 20;
        ps.push_back({pos, vel, u8(ship.team())});
    }
    mShipPlot->setLocations( std::move(ps) );

    for(auto& s : mShots)
    {
        s->remove();
    }
    mShots.clear();
    for(auto& proj : mState.getProjectiles())
    {
        icore::vector3df pos{proj.position().x.value, proj.position().y.value, proj.position().z.value};
        pos *= 20;
        icore::vector3df vel{proj.velocity().x.value, proj.velocity().y.value, proj.velocity().z.value};
        vel *= 20;
        auto shotfx = new scene::ShotFx( mDevice->getSceneManager()->getRootSceneNode(), mDevice->getSceneManager() );
        shotfx->setShot( pos, vel );
        mShots.push_back( shotfx );
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
                    auto bb = mDevice->getSceneManager()->addBillboardSceneNode();
                    bb->setPosition(20 * icore::vector3df{pos.x.value, pos.y.value, pos.z.value});
                    bb->setMaterialFlag(video::EMF_LIGHTING, false);
                    bb->setSize(s, s, s);
                    bb->setMaterialType(video::EMT_TRANSPARENT_ALPHA_CHANNEL);
                    bb->setMaterialTexture(0, mDevice->getVideoDriver()->getTexture("data/attack.png"));
                    bb->addAnimator(mDevice->getSceneManager()->createDeleteAnimator(1000));
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
