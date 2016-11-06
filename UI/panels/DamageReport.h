//
// Created by erik on 11/4/16.
//

#ifndef SPATACS_DAMAGEREPORT_H
#define SPATACS_DAMAGEREPORT_H

#include "game/Starship.h"
#include "UI/IrrRAII.h"
#include "UI/gfx/HealthBar.h"
#include <irrlicht/IGUIElement.h>
#include <vector>

namespace spatacs
{
    namespace ui
    {
        class DamageReport : public irr::gui::IGUIElement
        {
        public:
            DamageReport(irr::gui::IGUIEnvironment* environment, irr::gui::IGUIElement* parent, irr::s32 id,
                                     const irr::core::rect<irr::s32>& rectangle);

            void update(const game::Starship& ship);
        private:
            std::vector<remove_ptr<irr::gui::HealthBar>> mHealthBars;
        };
    }
}

#endif //SPATACS_DAMAGEREPORT_H
