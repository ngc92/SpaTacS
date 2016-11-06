#include "HealthBar.h"
#include <irrlicht/IVideoDriver.h>
#include <irrlicht/IGUIEnvironment.h>
#include <irrlicht/IGUIFont.h>
#include <iostream>

using namespace irr;
using namespace gui;

HealthBar::HealthBar( gui::IGUIEnvironment* guienv, IGUIElement* parent, s32 id, core::recti rect )
    : IGUIElement(gui::EGUIET_ELEMENT, guienv, parent, id, rect),
    mHealthyColor(255, 64, 128, 0),
    mDamageColor(255, 64, 0, 0),
    mTextColor(255, 20, 20, 0)
{
    core::recti sub( core::position2di(0,0), core::position2di(rect.getWidth(), rect.getHeight()) );
    Environment->addStaticText( L"", sub, false, false, this);
}

HealthBar::~HealthBar()
{
}

void HealthBar::draw()
{
    video::IVideoDriver* driver = Environment->getVideoDriver();
    auto rect = AbsoluteRect;

    driver->setTransform(video::ETS_WORLD, core::matrix4{});
    driver->draw2DRectangle( mDamageColor, rect );
    rect.LowerRightCorner.X -= rect.getWidth() * (1.f - mStatus);
    driver->draw2DRectangle( mHealthyColor, rect );

    Environment->getBuiltInFont()->draw(getText(), rect + irr::core::vector2di(4, 0), mTextColor, false, true);

    IGUIElement::draw();
}

void HealthBar::OnPostRender(irr::u32 time)
{
}

//! Writes attributes of the element.
//! Implement this to expose the attributes of your element for
//! scripting languages, editors, debuggers or xml serialization purposes.
void HealthBar::serializeAttributes(io::IAttributes* out, io::SAttributeReadWriteOptions* options=0) const
{
    IGUIElement::serializeAttributes(out, options);

    out->addFloat ("Status",      mStatus);
    out->addColor ("HealthColor", mHealthyColor);
    out->addColor ("DamageColor", mDamageColor);
    out->addColor ("TextColor",   mTextColor);
}


//! Reads attributes of the element
void HealthBar::deserializeAttributes(io::IAttributes* in, io::SAttributeReadWriteOptions* options=0)
{
    IGUIElement::deserializeAttributes(in, options);

    mStatus       = in->getAttributeAsFloat("Status");
    mHealthyColor = in->getAttributeAsColor("HealthColor");
    mDamageColor  = in->getAttributeAsColor("DamageColor");
    mTextColor    = in->getAttributeAsColor("TextColor");

}

