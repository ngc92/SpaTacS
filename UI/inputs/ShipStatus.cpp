//
// Created by erik on 10/21/16.
//

#include "game/systems.h"
#include "UI/gfx/ShipStatusUI.h"
#include "ShipStatus.h"
#include "game/Starship.h"
#include <irrlicht/IGUIEnvironment.h>
#include <algorithm>
#include <sstream>
#include <iomanip>

using namespace spatacs;
using namespace ui;

ShipStatusUI::ShipStatusUI(irr::gui::ShipStatusUI* rp) :
        mGUIElement( rp )
{

}

void ShipStatusUI::update(const game::Starship& ship)
{
    mGUIElement->setShipName( ship.name() );
    mGUIElement->clearSystems();

    mGUIElement->pushSystem(irr::gui::SystemStatus{"shield", ship.shield(), ship.max_shield()});
    mGUIElement->pushSystem(irr::gui::SystemStatus{"hull", ship.armour(), ship.max_armour()});
    mGUIElement->pushSystem(irr::gui::SystemStatus{"structure", ship.hp(), ship.max_hp()});

    if(mShowFuel) {
        game::TankInfo ti;
        ship.components().apply(ti);
        mGUIElement->pushSystem(irr::gui::SystemStatus{"fuel", ti.fuel() / 1.0_t, ti.capacity() / 1.0_t});
    }

    if(mShowAmmunition) {
        game::ListAmmunition la;
        ship.components().apply(la);
        std::size_t amm = 0;
        for (auto& a : la) {
            amm += a.amount;
        }
        mGUIElement->pushSystem(irr::gui::SystemStatus{"ammo", amm, la.capacity()});
    }

    if(mShowPower) {
        double p = (ship.getProducedEnergy() / 0.1_s) / 1.0_MW;
        double u = (ship.getUsedEnergy() / 0.1_s) / 1.0_MW;
        mGUIElement->pushSystem(irr::gui::SystemStatus{"power", p - u, p});
    }
}

void ShipStatusUI::setShowFuel(bool sf)
{
    mShowFuel = sf;
}

void ShipStatusUI::setShowSystemHealth(bool sh)
{
}

void ShipStatusUI::setVisible(bool v)
{
    mGUIElement->setVisible(v);
}

void ShipStatusUI::setShowAmmunition(bool sa)
{
    mShowAmmunition = sa;
}

void ShipStatusUI::setShowPower(bool sp)
{
    mShowPower = sp;
}





