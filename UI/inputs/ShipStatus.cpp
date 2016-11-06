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
            amm += a.amount;
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
            const auto& ammo_type = wpn.get<game::ProjectileWpnData>();
            std::size_t ammo = 0;
            game::AmmoData dat;
            for (auto& a : la) {
                if(a.data.name == ammo_type.mAmmo) {
                    ammo += a.amount;
                    dat = a.data;
                }
            }
            std::wstringstream str;
            str << std::setprecision(2);
            str << L"weapon " << (i+1) << L"\n";
            str << L" " << ammo_type.mAmmo.c_str() << L" (" << ammo << L")\n";
            str << L" " << dat.mass << " @ " << sqrt(2.0*dat.charge / dat.mass) << "\n";
            str << L" " << dat.damage.shield_overload << L" | " << dat.damage.high_explosive <<
                " | " << dat.damage.armour_pierce << L"\n";
            (*viewer)->setText(str.str().c_str());
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

void ShipStatusUI::addWeaponStatus(irr::gui::IGUIStaticText* wsui)
{
    wsui->setOverrideColor( irr::video::SColor(255, 255, 255, 255));
    mWpnStatusView.push_back( remove_ptr<irr::gui::IGUIElement>(wsui) );
}

void ShipStatusUI::setShowPower(bool sp)
{
    mShowPower = sp;
}

void ShipStatusUI::setShowWeapons(bool sw)
{
    mShowWeapons = sw;
}





