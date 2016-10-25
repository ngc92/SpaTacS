//
// Created by erik on 10/22/16.
//

#ifndef SPATACS_WEAPONSTATUSUI_H
#define SPATACS_WEAPONSTATUSUI_H

#include <irrlicht/IGUIElement.h>
#include <string>

namespace irr
{
    namespace gui
    {
        class WeaponStatusUI : public IGUIElement
        {
        public:
            WeaponStatusUI(IGUIEnvironment* environment, IGUIElement* parent, s32 id, const core::rect <s32>& rectangle);

            virtual void OnPostRender(u32 timeMs) override;
            virtual bool OnEvent(const SEvent& event) override;

            // info setting
            void setWeaponName(std::string wn);
            void setAmmoAmount(std::size_t count);
            void setAmmoType(std::string tp);
        private:
            irr::gui::IGUIElement* mTextInfo;

            std::string mWpnName;
            std::string mAmmoType;
            std::size_t mAmmoCount;
        };
    }
}

#endif //SPATACS_WEAPONSTATUSUI_H
