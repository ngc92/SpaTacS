//
// Created by erik on 10/23/16.
//

#include "WeaponStatusUI.h"
#include <irrlicht/IGUIEnvironment.h>
#include <irrlicht/IGUIStaticText.h>
#include <sstream>
#include <iostream>

using namespace irr;
using namespace gui;

WeaponStatusUI::WeaponStatusUI(IGUIEnvironment* environment, IGUIElement* parent, s32 id, const core::rect <s32>& rectangle)
        : IGUIElement(EGUIET_ELEMENT, environment, parent, id,
                      rectangle)
{
    auto text = environment->addStaticText(L"Weapon", rectangle - rectangle.UpperLeftCorner, false, false, this);
    text->setOverrideColor( video::SColor(255, 255, 255, 255));
    mTextInfo = text;
}

void WeaponStatusUI::OnPostRender(u32 timeMs)
{
    std::wstringstream str;
    str << mWpnName.c_str() << "\n";
    str << "ammo: " << mAmmoType.c_str() << "\n";
    str << mAmmoCount << "\n";
    mTextInfo->setText(str.str().c_str());
    IGUIElement::OnPostRender(timeMs);
}

bool WeaponStatusUI::OnEvent(const SEvent& event)
{
    return IGUIElement::OnEvent(event);
}

void WeaponStatusUI::setWeaponName(std::string wn)
{
    mWpnName = std::move(wn);
}

void WeaponStatusUI::setAmmoAmount(std::size_t count)
{
    mAmmoCount = count;
}

void WeaponStatusUI::setAmmoType(std::string tp)
{
    mAmmoType = std::move(tp);
}