//
// Created by erik on 10/1/16.
//

#ifndef SPATACS_CORE_SIMULATION_BASE_H
#define SPATACS_CORE_SIMULATION_BASE_H

#include <events/IEvent.h>

namespace spatacs
{
namespace core
{
    class GameStateBase;

    class SimulationBase
    {
        void cleanup();

        virtual void update() = 0;
        virtual void eventLoop() = 0;

    protected:
        using EventPtr = std::unique_ptr<events::IEvent>;
        using EventVec = std::vector<EventPtr>;
        using StatePtr = std::unique_ptr<GameStateBase>;

        // thread internal variables, don't touch from outside!
        std::unique_ptr<GameStateBase> mState;

        void addEvent(EventPtr e );

        EventVec mAllEvents;
        // thread internal variables, don't touch from outside!
        EventVec mEventCache;
    public:
        SimulationBase();
        virtual ~SimulationBase();

        EventVec step(EventVec inEvents);
        StatePtr extractState() const;
    };
}
}


#endif //SPATACS_CORE_SIMULATION_BASE_H
