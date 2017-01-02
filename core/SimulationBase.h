//
// Created by erik on 10/1/16.
//

#ifndef SPATACS_CORE_SIMULATION_BASE_H
#define SPATACS_CORE_SIMULATION_BASE_H

#include <vector>
#include "any_vector.h"

namespace spatacs
{
namespace core
{
    class GameStateBase;

    class SimulationBase
    {
    protected:
        using EventVec  = any_vector;
        using NotifyVec = any_vector;

    public:
        SimulationBase() = default;
        virtual ~SimulationBase() = default;

        virtual void processInput(EventVec inEvents) = 0;
        virtual NotifyVec update() = 0;
        virtual const GameStateBase& getState() const = 0;

    };
}
}


#endif //SPATACS_CORE_SIMULATION_BASE_H
