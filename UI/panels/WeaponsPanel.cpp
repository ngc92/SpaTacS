//
// Created by erik on 11/10/16.
//

#include "WeaponsPanel.h"
#include <sstream>
#include <iomanip>
#include <irrlicht/IGUIEnvironment.h>
#include <irrlicht/IGUIStaticText.h>
#include "game/systems/Ammunition.h"
#include "game/SubSystems.h"
#include "game/systems/make_system.h"

using spatacs::ui::WeaponsPanel;


void WeaponsPanel::update(const game::Starship& ship)
{
    mWeapons.clear();

    game::systems::ListAmmunition la;
    ship.components().apply(la);


    int i = 1;
    int x0 = 0;
    auto wpn_view = game::systems::make_system<const game::ProjectileWpnData>(
            [&](const game::ProjectileWpnData& wpn_data)
            {
                std::size_t ammo = 0;
                game::AmmoData dat;
                for (auto& a : la) {
                    if(a.data.name == wpn_data.mAmmo) {
                        ammo += a.amount;
                        dat = a.data;
                    }
                }
                std::wstringstream str;
                str << std::setprecision(2);
                str << L"weapon " << (i) << L"\n";
                str << L" " << wpn_data.mAmmo.c_str() << L" (" << ammo << L")\n";
                str << L" " << dat.mass << " @ " << sqrt(2.0*dat.charge / dat.mass) << "\n";
                str << L" " << dat.damage.shield_overload << L" | " << dat.damage.high_explosive <<
                    " | " << dat.damage.armour_pierce << L"\n";

                auto text = Environment->addStaticText(str.str().c_str(),
                                                       irr::core::recti(x0, 0, x0+90, 40),
                                                       false, true, this
                );
                text->setOverrideColor( irr::video::SColor(255, 255, 255, 255));
                mWeapons.push_back( remove_ptr<irr::gui::IGUIElement>(text) );
                x0 += 100;

                i += 1;
            }
    );

    ship.components().apply(wpn_view);
}

WeaponsPanel::WeaponsPanel(irr::gui::IGUIEnvironment* environment, irr::gui::IGUIElement* parent, irr::s32 id,
                           const irr::core::rect<irr::s32>& rectangle)
        : IGUIElement(irr::gui::EGUIET_ELEMENT, environment, parent, id, rectangle)
{}
