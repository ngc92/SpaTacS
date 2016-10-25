//
// Created by erik on 10/21/16.
//

#ifndef SPATACS_SHIPSTATUS_H
#define SPATACS_SHIPSTATUS_H

#include "UI/IrrRAII.h"
#include <irrlicht/rect.h>

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

            void addWeaponStatus(irr::gui::WeaponStatusUI* wsui);

            void setShowFuel( bool sf );
            void setShowShipStatus( bool ss );
            void setShowSystemHealth( bool sh );
            void setShowAmmunition( bool sa );
            void setShowPower( bool sp );
            void setShowWeapons( bool sw );

            void setVisible( bool v );
        private:
            remove_ptr<irr::gui::ShipStatusUI> mGUIElement;

            std::vector<remove_ptr<irr::gui::WeaponStatusUI>> mWpnStatusView;

            // configuration
            bool mShowFuel         = true;
            bool mShowShipStatus   = true;
            bool mShowSystemHealth = true;
            bool mShowAmmunition   = true;
            bool mShowPower        = true;
            bool mShowWeapons      = true;
        };
    }
}

#endif //SPATACS_SHIPSTATUS_H
