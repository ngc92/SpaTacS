//
// Created by erik on 9/20/16.
//

#ifndef SOI_IRRLICHTUI_H
#define SOI_IRRLICHTUI_H

#include <irrlicht/IGUIElement.h>
#include "IUI.h"
#include "cmd/CommandManager.h"

namespace irr
{
    class IrrlichtDevice;
    namespace video
    {
        class IVideoDriver;
    }
    namespace scene
    {
        class LocationPlotter3D;
        class ISceneNode;
    }
}

namespace spatacs {
    namespace ui {

        struct Explosion
        {
            Vec pos;
            float timer;
            float size;
        };
        class IrrlichtUI : public IUI
        {
        public:
            IrrlichtUI(std::uint64_t team);
            ~IrrlichtUI();
            void init() override;

            std::vector<cmd::Command> getCommands() const override;

            void setState(const core::GameState& state) override;

            bool step() override;

            void notifyEvents(const std::vector<std::unique_ptr<events::IEvent>>& events) override;

            // interface for tools
            const core::GameState& state() const { return mState; }
            void addCommand( cmd::Command c );

            irr::IrrlichtDevice* getDevice() { return mDevice; }

        private:
            std::uint64_t mOwnTeam;
            irr::IrrlichtDevice* mDevice;
            core::GameState mState;
            std::vector<irr::scene::ISceneNode*> mShots;
            class EventRec;
            std::unique_ptr<EventRec> mEventReceiver;

            irr::scene::LocationPlotter3D* mShipPlot;
            cmd::CommandManager mCommands;
        };
    }
}


#endif //SOI_IRRLICHTUI_H
