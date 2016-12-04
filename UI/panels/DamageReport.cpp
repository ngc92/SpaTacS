//
// Created by erik on 11/4/16.
//

#include "DamageReport.h"
#include "game/systems/make_system.h"
#include "game/SubSystems.h"

using spatacs::ui::DamageReport;

void DamageReport::update(const game::Starship& ship)
{
    mHealthBars.clear();

    auto W = getRelativePosition().getWidth();
    auto H = getRelativePosition().getHeight();

    auto sys = game::systems::make_system<const game::Health, const game::Name>
            (       [this, W](const game::Health& h, const game::Name& n) mutable
                    {
                        remove_ptr<irr::gui::HealthBar> rptr;
                        rptr.reset(new irr::gui::HealthBar(Environment, this, 0, irr::core::recti(0, 0, W, 15)) );
                        rptr->setStatus( h.current / h.maximum );
                        rptr->setText( irr::core::stringw(n.name.c_str()).c_str() );
                        mHealthBars.push_back( std::move(rptr) );
                    }
            );
    ship.components().apply(sys);

    if(mHealthBars.size() > 0) {
        irr::core::position2di offset{0, 0};
        int bar_h = H - mHealthBars.size() + 1;
        int hpb = bar_h / mHealthBars.size();
        for (auto& hb : mHealthBars) {
            offset.Y += hpb - 15;
            hb->setRelativePosition(hb->getRelativePosition() + offset);
        }
    }
}

DamageReport::DamageReport(irr::gui::IGUIEnvironment* environment, irr::gui::IGUIElement* parent, irr::s32 id,
                           const irr::core::rect<irr::s32>& rectangle)
        : IGUIElement(irr::gui::EGUIET_ELEMENT, environment, parent, id, rectangle)
{}
