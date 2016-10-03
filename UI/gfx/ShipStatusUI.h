//
// Created by erik on 10/3/16.
//

#ifndef SPATACS_SYSTEMSTATUSUI_H
#define SPATACS_SYSTEMSTATUSUI_H

#include <irrlicht/IGUIElement.h>
#include <string>
#include <vector>

namespace irr
{
    namespace gui
    {
        struct SystemStatus
        {
            std::string name;
            float hp;
            float max_hp;
        };

        class SystemStatusUI : public IGUIElement
        {
        public:
            SystemStatusUI(IGUIEnvironment* environment, IGUIElement* parent, s32 id, const core::rect <s32>& rectangle);

            virtual void draw() override;

            virtual void OnPostRender(u32 timeMs) override;

            virtual bool OnEvent(const SEvent& event) override;

            void clearSystems();
            void pushSystem( SystemStatus sys );
        private:
            std::vector<SystemStatus> mSystems;
        };
    }
}


#endif //SPATACS_SYSTEMSTATUSUI_H
