//
// Created by erik on 10/1/16.
//

#ifndef SPATACS_CORE_SIMULATION_BASE_H
#define SPATACS_CORE_SIMULATION_BASE_H

#include <events/IEvent.h>
#include <vector>

namespace spatacs
{
namespace core
{
    class GameStateBase;

    class SimulationBase
    {
    protected:
        using EventPtr = std::unique_ptr<events::IEvent>;
        using EventVec = std::vector<EventPtr>;

    public:
        SimulationBase() = default;
        virtual ~SimulationBase() = default;

        virtual void processInput(EventVec inEvents) = 0;
        virtual EventVec update() = 0;
        virtual const GameStateBase& getState() const = 0;

    };
}
}


#endif //SPATACS_CORE_SIMULATION_BASE_H
