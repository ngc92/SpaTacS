//
// Created by erik on 10/3/16.
//

#include "ShipStatusUI.h"
#include <irrlicht/IGUIEnvironment.h>
#include <irrlicht/IVideoDriver.h>
#include <irrlicht/IGUIFont.h>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <cmath>

using irr::video::SColor;
using namespace irr::gui;

void ShipStatusUI::draw()
{
    auto p = getAbsolutePosition().UpperLeftCorner;
    auto clip = getAbsoluteClippingRect();
    irr::core::recti rect {p, p + irr::core::vector2di(getAbsolutePosition().getWidth(), 17)};
    Environment->getBuiltInFont()->draw(mShipName.c_str(), rect, video::SColor(255, 255, 255, 255), false, false, &clip);
    rect += irr::core::vector2di(1, 16);

    for(auto& sys : mSystems)
    {
        Environment->getVideoDriver()->draw2DRectangle(SColor(255, 64, 0, 0), rect, &clip);
        auto cp = rect;
        cp.LowerRightCorner.X = cp.UpperLeftCorner.X + (int)std::round(rect.getWidth() * sys.hp / sys.max_hp);
        Environment->getVideoDriver()->draw2DRectangle(SColor(255, 64, 128, 0), cp, &clip);
        std::wstringstream stream;        
        stream << std::fixed << std::setprecision(1);
        stream << sys.name.c_str() << " " << sys.hp << "/" << sys.max_hp;
        Environment->getBuiltInFont()->draw(stream.str().c_str(), rect, video::SColor(255, 20, 20, 0), false, true, &clip);
        rect += irr::core::vector2di(0, 19);
    }
}

void ShipStatusUI::OnPostRender(irr::u32 timeMs)
{
    IGUIElement::OnPostRender(timeMs);
}

bool ShipStatusUI::OnEvent(const irr::SEvent& event)
{
    return IGUIElement::OnEvent(event);
}

ShipStatusUI::ShipStatusUI(IGUIEnvironment* environment, IGUIElement* parent, s32 id, const core::rect<s32>& rectangle)
        : IGUIElement(EGUIET_ELEMENT, environment, parent, id,
                      rectangle)
{}

void ShipStatusUI::pushSystem(SystemStatus sys)
{
    mSystems.push_back( sys );
}

void ShipStatusUI::clearSystems()
{
    mSystems.clear();
}

void ShipStatusUI::setShipName(std::string name)
{
    mShipName = std::move(name);
}
