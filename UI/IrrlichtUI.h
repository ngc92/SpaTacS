//
// Created by erik on 9/20/16.
//

#ifndef SOI_IRRLICHTUI_H
#define SOI_IRRLICHTUI_H

#include "IUI.h"
#include "IrrRAII.h"
#include "core/GameState.h"

namespace irr
{
    class IrrlichtDevice;
    class SEvent;
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
    namespace cmd
    {
        class CommandManager;
    };

    namespace ui {
        class IrrlichtUI : public IUI
        {
        public:
            IrrlichtUI(std::uint64_t team, irr::IrrlichtDevice* device, std::shared_ptr<cmd::CommandManager> cmd);
            ~IrrlichtUI();
            void init() override;

            virtual void getCommandEvents(std::vector<events::EventPtr>& evts) override;

            void setState(const std::shared_ptr<const core::GameState>& state) override;

            bool step() override;

            void notifyEvents(const std::vector<std::unique_ptr<events::IEvent>>& events) override;

            bool handleUIEvent( const irr::SEvent& ev );

            // interface for tools
            const core::GameState& state() const { return *mState; }
            cmd::CommandManager& getCommandMgr();
            irr::IrrlichtDevice* getDevice() { return mDevice; }
            bool pause() const;
        private:
            std::uint64_t mOwnTeam;
            irr::IrrlichtDevice* mDevice;
            std::shared_ptr<const core::GameState> mState;

            class EventRec;
            std::unique_ptr<EventRec> mEventReceiver;

            irr::scene::ISceneNode* mMap;
            std::shared_ptr<cmd::CommandManager> mCommands;
        };
    }
}


#endif //SOI_IRRLICHTUI_H
