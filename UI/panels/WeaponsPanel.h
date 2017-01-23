//
// Created by erik on 11/10/16.
//

#ifndef SPATACS_WEAPONSPANEL_H
#define SPATACS_WEAPONSPANEL_H

#include "UI/IrrRAII.h"
#include "UI/gfx/HealthBar.h"
#include <irrlicht/IGUIElement.h>
#include <vector>

namespace spatacs
{
    namespace ui
    {
        class WeaponsPanel : public irr::gui::IGUIElement
        {
        public:
            WeaponsPanel(irr::gui::IGUIEnvironment* environment, irr::gui::IGUIElement* parent, irr::s32 id,
                         const irr::core::rect<irr::s32>& rectangle);

            void update(const game::Starship& ship);
        private:
            std::vector<remove_ptr<irr::gui::IGUIElement>> mWeapons;
        };
    }
}


#endif //SPATACS_WEAPONSPANEL_H
