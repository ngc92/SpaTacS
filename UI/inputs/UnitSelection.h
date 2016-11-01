//
// Created by erik on 9/23/16.
//

#ifndef SOI_UNITSELECTION_H
#define SOI_UNITSELECTION_H

#include <memory>
#include <irrlicht/IGUIElement.h>
#include "UI/IInputMode.h"
#include <set>
#include "UI/IrrRAII.h"

namespace spatacs {
    namespace ui {
        class UnitSelection : public IInputMode
        {
        public:
            UnitSelection(std::uint64_t team);

        protected:
            void init(irr::gui::IGUIEnvironment* guienv, irr::scene::ISceneManager* smgr) override;

        public:
            void onLeftClick(ray_t ray, const irr::SEvent::SMouseInput& event) override;
            void onMouseMove(ray_t ray, const irr::SEvent::SMouseInput& event) override;
            void onRightClick(ray_t ray, const irr::SEvent::SMouseInput& event) override;
            void onWheel(float scroll) override;
            void onLeftMouseUp(ray_t ray, const irr::SEvent::SMouseInput& event) override;

            void onKeyPress( irr::EKEY_CODE key ) override;
            void onKeyRelease( irr::EKEY_CODE key ) override;

            void step() override;
        private:
            std::uint64_t mOwnTeam;
            std::shared_ptr<IInputMode> mChildMode;
            remove_ptr<irr::gui::IGUIElement> mHoverUI   = nullptr;
            remove_ptr<irr::gui::IGUIElement> mSelectBox = nullptr;
            ray_t mCurrentRay;

            irr::core::vector2di mDragStart;

            std::set<irr::EKEY_CODE> mKeysDown;
        };
    }
}


#endif //SOI_UNITSELECTION_H
