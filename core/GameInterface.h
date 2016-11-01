//
// Created by erik on 9/19/16.
//

#ifndef SOI_IUI_H
#define SOI_IUI_H

#include <vector>
#include "events/IEvent.h"

namespace spatacs
{
    namespace events
    {
        class IEvent;
    }
    namespace core
    {
        class GameInterface
        {
        public:
            virtual void init() = 0;
            /// this function gets all the events that this interface wants to transfer
            /// into the simulation.
            /// \param evts Push your events into this vector!
            virtual void getCommandEvents(std::vector<events::EventPtr>& evts) = 0;

            virtual void setState(const std::shared_ptr<const core::GameState>& state) = 0;
            virtual void notifyEvents(const std::vector<std::unique_ptr<events::IEvent>>& events) = 0;
            virtual bool step() = 0;
        private:
        };
    }
}

#endif //SOI_IUI_H
