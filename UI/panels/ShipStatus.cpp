//
// Created by erik on 10/21/16.
//

#include "UI/gfx/HealthBar.h"
#include "ShipStatus.h"
#include "game/Starship.h"
#include "game/systems/TankInfo.h"
#include "game/systems/Ammunition.h"
#include "game/SubSystems.h"
#include <irrlicht/IGUIEnvironment.h>
#include <algorithm>
#include <sstream>
#include <iomanip>

using namespace spatacs;
using namespace ui;

ShipStatus::ShipStatus(irr::gui::IGUIEnvironment* environment, irr::gui::IGUIElement* parent, irr::s32 id,
                       const irr::core::rect<irr::s32>& rectangle)
        : IGUIElement(irr::gui::EGUIET_ELEMENT, environment, parent, id, rectangle)
{
    auto pos = getAbsolutePosition();
    auto w = pos.getWidth();
    irr::core::recti rect(0, 0, w, 15);
    mName.reset(Environment->addStaticText(L"", rect, false, false, this));
}

void ShipStatus::update(const game::Starship& ship)
{
    struct Sys
    {
        std::wstring name;
        double status;
    };

    std::vector<Sys> bars;

    mBars.clear();
    mName->setText( irr::core::stringw(ship.name().c_str()).c_str() );

    bars.push_back( Sys{L"shield", ship.shield() / ship.max_shield()} );
    bars.push_back( Sys{L"hull", ship.armour() / ship.max_armour()} );
    bars.push_back( Sys{L"structure", ship.hp() / ship.max_hp()} );

    if(mShowFuel) {
        game::systems::TankInfo ti;
        ship.components().apply(ti);
        bars.push_back( Sys{L"fuel", ti.fuel() / ti.capacity()} );
    }

    if(mShowAmmunition) {
        game::systems::ListAmmunition la;
        ship.components().apply(la);
        std::size_t amm = 0;
        for (auto& a : la) {
            amm += a.amount;
        }
        bars.push_back( Sys{L"ammo", (double)amm / la.capacity()} );
    }

    if(mShowPower) {
        double p = (ship.getProducedEnergy() / 0.1_s) / 1.0_MW;
        double u = (ship.getUsedEnergy() / 0.1_s) / 1.0_MW;
        bars.push_back(Sys{L"power", p/u-1.0});
    }
    auto pos = getAbsolutePosition();
    auto w = pos.getWidth();
    irr::core::recti rect(0, 17, w, 15+17);
    for(auto& b : bars)
    {
        remove_ptr<irr::gui::HealthBar> rptr;
        auto hb = new irr::gui::HealthBar(Environment, this, -1, rect);
        hb->setStatus( b.status );
        hb->setText( b.name.c_str() );
        rect += irr::core::vector2di(0, 17);
        rptr.reset(hb);
        mBars.push_back(std::move(rptr));
    }
}

void ShipStatus::setShowFuel(bool sf)
{
    mShowFuel = sf;
}

void ShipStatus::setShowAmmunition(bool sa)
{
    mShowAmmunition = sa;
}

void ShipStatus::setShowPower(bool sp)
{
    mShowPower = sp;
}





