//
// Created by erik on 9/22/16.
//

#ifndef SOI_UNITMOVEMENTINPUT_H
#define SOI_UNITMOVEMENTINPUT_H

#include <cstdint>
#include <boost/optional.hpp>
#include "UI/IInputMode.h"
#include "vec.h"

namespace irr
{
    namespace gui
    {
        class ShipStatusUI;
    }
}

namespace spatacs {
    namespace ui {
        class UnitCommand : public IInputMode
        {
        public:
            UnitCommand(std::uint64_t id);
            ~UnitCommand();

            virtual void init(irr::gui::IGUIEnvironment* guienv) override;

            void onMouseMove(ray_t ray) override;
            void onRightClick(ray_t ray) override;
            void onWheel(float scroll) override;
            void onKeyPress(irr::EKEY_CODE key) override;

            void draw(irr::video::IVideoDriver* driver) override;

        private:
            boost::optional<length_vec> aim(const ray_t &ray) const;

            std::uint64_t mActiveShipID;
            boost::optional<length_vec> mCurrentAim;
            std::uint64_t mCurrentAimShip;
            length_t mTargetY     = 0.0_km;
            speed_t  mTargetSpeed = 0.75_kps;
            float mBaseY      = 0.f;

            irr::gui::IGUIElement*    mDistanceMarker = nullptr;
            irr::gui::ShipStatusUI*   mShipInfo       = nullptr;
            irr::gui::IGUIElement*    mTargetInfo     = nullptr;
            irr::gui::IGUIElement*    mSpeedInfo      = nullptr;

            enum Mode
            {ATTACK, MOVE} mMode = MOVE;
        };
    }
}

#endif //SOI_UNITMOVEMENTINPUT_H
