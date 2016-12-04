//
// Created by erik on 10/21/16.
//

#ifndef SPATACS_SHIPSTATUS_H
#define SPATACS_SHIPSTATUS_H

#include "UI/IrrRAII.h"
#include <vector>
#include <irrlicht/rect.h>
#include <irrlicht/IGUIStaticText.h>

namespace irr
{
    namespace gui
    {
        class HealthBar;
    }
}

namespace spatacs
{
    namespace game
    {
        class Starship;
    }
    namespace ui
    {
        class ShipStatus : public irr::gui::IGUIElement
        {
        public:
            ShipStatus(irr::gui::IGUIEnvironment* environment, irr::gui::IGUIElement* parent, irr::s32 id,
                       const irr::core::rect<irr::s32>& rectangle);
            void update(const game::Starship& ship);

            void setShowFuel( bool sf );
            void setShowAmmunition( bool sa );
            void setShowPower( bool sp );
        private:
            remove_ptr<irr::gui::IGUIElement> mName;
            std::vector<remove_ptr<irr::gui::HealthBar>> mBars;

            // configuration
            bool mShowFuel         = true;
            bool mShowAmmunition   = true;
            bool mShowPower        = true;
        };
    }
}

#endif //SPATACS_SHIPSTATUS_H
