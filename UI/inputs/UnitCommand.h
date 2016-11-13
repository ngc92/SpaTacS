//
// Created by erik on 9/22/16.
//

#ifndef SOI_UNITMOVEMENTINPUT_H
#define SOI_UNITMOVEMENTINPUT_H

#include <cstdint>
#include <boost/optional.hpp>
#include "UI/IrrRAII.h"
#include "UI/IInputMode.h"
#include "vec.h"

namespace irr
{
    namespace gui
    {
        class ShipStatusUI;
    }

    namespace scene
    {
        class ISceneNode;
        class MultiLineNode;
    }
}

namespace spatacs {
    namespace ui {
        class ShipStatus;
        class DamageReport;
        class WeaponsPanel;

        class UnitCommand : public IInputMode
        {
        public:
            UnitCommand(game::ObjectID id);
            ~UnitCommand();

            virtual void init(irr::gui::IGUIEnvironment* guienv, irr::scene::ISceneManager* smgr) override;

            void onMouseMove(ray_t ray, const irr::SEvent::SMouseInput& event) override;
            void onRightClick(ray_t ray, const irr::SEvent::SMouseInput& event) override;
            void onWheel(float scroll) override;
            void onKeyPress(irr::EKEY_CODE key) override;

            void step() override;

        private:
            boost::optional<length_vec> aim(const ray_t &ray) const;

            game::ObjectID mActiveShipID{};
            boost::optional<length_vec> mCurrentAim;
            game::ObjectID mCurrentAimShip{};
            length_t mTargetY     = 0.0_km;
            speed_t  mTargetSpeed = 0.75_kps;
            float mBaseY      = 0.f;

            remove_ptr<irr::gui::IGUIElement>  mDistanceMarker;
            remove_ptr<ShipStatus>             mShipStatus;
            remove_ptr<ShipStatus>             mTargetStatus;
            remove_ptr<irr::gui::IGUIElement>  mSpeedInfo;
            remove_ptr<irr::scene::MultiLineNode> mTrajectoryPlotter;
            remove_ptr<DamageReport>           mDamageReport;
            remove_ptr<WeaponsPanel>           mWeaponsPanel;

            enum Mode
            {ATTACK, MOVE} mMode = MOVE;
        };
    }
}

#endif //SOI_UNITMOVEMENTINPUT_H
