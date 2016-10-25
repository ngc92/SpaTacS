//
// Created by erik on 10/21/16.
//

#include "game/systems.h"
#include "UI/gfx/ShipStatusUI.h"
#include "UI/gfx/WeaponStatusUI.h"
#include "ShipStatus.h"
#include "game/Starship.h"
#include <irrlicht/IGUIEnvironment.h>
#include <algorithm>

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

    if(mShowSystemHealth) {
        auto sys = core::make_system<const game::ComponentEntity, const game::Health, const game::Name>
        (
                [this](const game::Health& h, const game::Name& n) mutable
                {
                    irr::gui::SystemStatus status{n.name, h.current, h.maximum};
                    mGUIElement->pushSystem( status );
                }
        );
        ship.components().apply(sys);
    }

    if(mShowShipStatus) {
        mGUIElement->pushSystem(irr::gui::SystemStatus{"shield", ship.shield(), ship.max_shield()});
        mGUIElement->pushSystem(irr::gui::SystemStatus{"hull", ship.hull_status().current, ship.hull_status().max});
        mGUIElement->pushSystem(irr::gui::SystemStatus{"structure", ship.hp(), ship.max_hp()});
    }
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
            amm += a.second;
        }
        mGUIElement->pushSystem(irr::gui::SystemStatus{"ammo", amm, la.capacity()});
    }

    if(mShowPower) {
        double p = (ship.getProducedEnergy() / 0.1_s) / 1.0_MW;
        double u = (ship.getUsedEnergy() / 0.1_s) / 1.0_MW;
        mGUIElement->pushSystem(irr::gui::SystemStatus{"power", p - u, p});
    }

    // weapons
    if(mShowWeapons)
    {
        game::ListAmmunition la;
        ship.components().apply(la);

        auto viewer = mWpnStatusView.begin();
        auto wc     = ship.weapon_count();
        for(unsigned i = 0; i < wc && viewer != mWpnStatusView.end(); ++i, ++viewer)
        {
            const auto& wpn = ship.weapon(i);
            (*viewer)->setWeaponName( "weapon " + std::to_string(i+1) );
            const auto& ammo_type = wpn.get<game::ProjectileWpnData>();
            std::size_t ammo = 0;
            for (auto& a : la) {
                if(a.first == ammo_type.mAmmo) {
                    ammo += a.second;
                }
            }
            (*viewer)->setAmmoAmount(ammo);
            (*viewer)->setAmmoType(ammo_type.mAmmo);
            (*viewer)->setVisible(true);
        }

        // make unused wpns invisible
        std::for_each(viewer, mWpnStatusView.end(), [](auto& v){ v->setVisible(false); });
    }

}

void ShipStatusUI::setShowFuel(bool sf)
{
    mShowFuel = sf;
}

void ShipStatusUI::setShowShipStatus(bool ss)
{
    mShowShipStatus = ss;
}

void ShipStatusUI::setShowSystemHealth(bool sh)
{
    mShowSystemHealth = sh;
}

void ShipStatusUI::setVisible(bool v)
{
    mGUIElement->setVisible(v);
}

void ShipStatusUI::setShowAmmunition(bool sa)
{
    mShowAmmunition = sa;
}

void ShipStatusUI::addWeaponStatus(irr::gui::WeaponStatusUI* wsui)
{
    mWpnStatusView.push_back( remove_ptr<irr::gui::WeaponStatusUI>(wsui) );
}

void ShipStatusUI::setShowPower(bool sp)
{
    mShowPower = sp;
}

void ShipStatusUI::setShowWeapons(bool sw)
{
    mShowWeapons = sw;
}





