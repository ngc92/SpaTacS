//
// Created by erik on 9/19/16.
//

#ifndef SOI_IUI_H
#define SOI_IUI_H

#include <vector>
#include "events/IEvent.h"
#include "any_vector.h"

namespace spatacs
{
    namespace events
    {
        class IEvent;
    }
    namespace core
    {
        class GameStateBase;

        class GameInterface
        {
        public:
            using notify_t = any_vector;
            using state_t  = std::shared_ptr<const core::GameStateBase>;

            virtual void init() = 0;
            /// this function gets all the events that this interface wants to transfer
            /// into the simulation.
            /// \param evts Push your events into this vector!
            virtual void getCommandEvents(std::vector<events::EventPtr>& evts) = 0;

            /// Generic setState method, gets called with the \p state_t type.
            /// To use the provided data, this has to be downcasted to the actual
            /// state type used in the game simulation.
            virtual void setState(const state_t& state) = 0;
            virtual void notify(const notify_t& events) = 0;
            virtual bool step() = 0;
        private:
        };
    }
}

#endif //SOI_IUI_H
