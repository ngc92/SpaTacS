//
// Created by erik on 9/20/16.
//

#ifndef SOI_IRRLICHTUI_H
#define SOI_IRRLICHTUI_H

#include "IUI.h"
#include "cmd/CommandManager.h"
#include "IrrRAII.h"
#include "core/GameState.h"

namespace irr
{
    class IrrlichtDevice;
    namespace video
    {
        class IVideoDriver;
    }
    namespace scene
    {
        class ISceneNode;
    }
}

namespace spatacs {
    namespace ui {
        class IrrlichtUI : public IUI
        {
        public:
            IrrlichtUI(std::uint64_t team);
            ~IrrlichtUI();
            void init() override;

            virtual void getCommandEvents(std::vector<events::EventPtr>& evts) const override;

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

            class EventRec;
            std::unique_ptr<EventRec> mEventReceiver;

            irr::scene::ISceneNode* mMap;
            cmd::CommandManager mCommands;
        };
    }
}


#endif //SOI_IRRLICHTUI_H
