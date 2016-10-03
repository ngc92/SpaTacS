//
// Created by erik on 9/19/16.
//

#ifndef SOI_IUI_H
#define SOI_IUI_H

#include <vector>
#include <core/GameState.h>
#include "cmd/Commands.h"

namespace spatacs
{
    namespace events
    {
        class IEvent;
    }
    namespace ui
    {
        class IUI
        {
        public:
            virtual void init() = 0;
            virtual std::vector<cmd::Command> getCommands() const = 0;
            virtual void setState( const core::GameState& state ) = 0;
            virtual void notifyEvents(const std::vector<std::unique_ptr<events::IEvent>>& events) = 0;
            virtual bool step() = 0;
        private:
        };
    }
}

#endif //SOI_IUI_H
