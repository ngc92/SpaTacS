//
// Created by erik on 9/23/16.
//

#ifndef SOI_UNITSELECTION_H
#define SOI_UNITSELECTION_H

#include <memory>
#include <irrlicht/IGUIElement.h>
#include "UI/IInputMode.h"

namespace spatacs {
    namespace ui {
        class UnitSelection : public IInputMode
        {
        public:
            UnitSelection(std::uint64_t team);

        protected:
            virtual void init(irr::gui::IGUIEnvironment* guienv) override;

        public:
            virtual void onLeftClick(ray_t ray) override;

            virtual void onMouseMove(ray_t ray) override;

            virtual void onRightClick(ray_t ray) override;

            virtual void onWheel(float scroll) override;

            virtual void onKeyPress( irr::EKEY_CODE key ) override;

            virtual void draw(irr::video::IVideoDriver* driver) override;

        private:
            std::uint64_t mOwnTeam;
            std::shared_ptr<IInputMode> mChildMode;
            irr::gui::IGUIElement* mHoverUI = nullptr;
            ray_t mCurrentRay;
        };
    }
}


#endif //SOI_UNITSELECTION_H
