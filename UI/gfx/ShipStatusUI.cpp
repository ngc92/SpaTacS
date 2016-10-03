//
// Created by erik on 10/3/16.
//

#include "SystemStatusUI.h"
#include <irrlicht/IGUIEnvironment.h>
#include <irrlicht/IVideoDriver.h>
#include <irrlicht/IGUIFont.h>

using irr::video::SColor;
using namespace irr::gui;

void SystemStatusUI::draw()
{
    auto p = getAbsolutePosition().UpperLeftCorner;
    auto clip = getAbsoluteClippingRect();
    irr::core::recti rect {p, p + irr::core::vector2di(getAbsolutePosition().getWidth(), 22)};
    for(auto& sys : mSystems)
    {
        Environment->getVideoDriver()->draw2DRectangle(SColor(255, 128, 128, 128), rect, &clip);
        Environment->getBuiltInFont()->draw(L"test", rect, video::SColor(255, 255, 255, 255), false, true, &clip);
        rect += irr::core::vector2di(0, 22);
    }
}

void SystemStatusUI::OnPostRender(irr::u32 timeMs)
{
    IGUIElement::OnPostRender(timeMs);
}

bool SystemStatusUI::OnEvent(const irr::SEvent& event)
{
    return IGUIElement::OnEvent(event);
}

SystemStatusUI::SystemStatusUI(IGUIEnvironment* environment, IGUIElement* parent, s32 id, const core::rect<s32>& rectangle)
        : IGUIElement(EGUIET_ELEMENT, environment, parent, id,
                      rectangle)
{}

void SystemStatusUI::pushSystem(SystemStatus sys)
{
    mSystems.push_back( sys );
}

void SystemStatusUI::clearSystems()
{
    mSystems.clear();
}
