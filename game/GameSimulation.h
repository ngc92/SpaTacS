//
// Created by erik on 11/15/16.
//

#ifndef SPATACS_GAMESIMULATION_H
#define SPATACS_GAMESIMULATION_H

#include "core/SimulationBase.h"

namespace spatacs { namespace game {


    class GameSimulation
            : public core::SimulationBase
    {
        void physics_callback(physics::PhysicsWorld& world, const physics::Object& A, const physics::Object& B,
                              physics::ImpactInfo info) override;

        void updateObjects() override;
        void updateShips() override;
    public:
        GameSimulation();
        virtual ~GameSimulation() = default;
    };

}}


#endif //SPATACS_GAMESIMULATION_H
