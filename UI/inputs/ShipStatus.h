//
// Created by erik on 10/21/16.
//

#ifndef SPATACS_SHIPSTATUS_H
#define SPATACS_SHIPSTATUS_H

#include "UI/IrrRAII.h"
#include <irrlicht/rect.h>
#include <irrlicht/IGUIStaticText.h>

namespace irr
{
    namespace gui
    {
        class IGUIElement;
        class ShipStatusUI;
        class WeaponStatusUI;
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
        class ShipStatusUI
        {
        public:
            ShipStatusUI(irr::gui::ShipStatusUI* rp);
            void update(const game::Starship& ship);

            void setShowFuel( bool sf );

            void setShowSystemHealth( bool sh );
            void setShowAmmunition( bool sa );
            void setShowPower( bool sp );

            void setVisible( bool v );
        private:
            remove_ptr<irr::gui::ShipStatusUI> mGUIElement;

            // configuration
            bool mShowFuel         = true;
            bool mShowAmmunition   = true;
            bool mShowPower        = true;
        };
    }
}

#endif //SPATACS_SHIPSTATUS_H
