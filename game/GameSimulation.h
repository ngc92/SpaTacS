//
// Created by erik on 11/15/16.
//

#ifndef SPATACS_GAMESIMULATION_H
#define SPATACS_GAMESIMULATION_H

#include "core/SimulationBase.h"
#include "physics/PhysicsWorld.h"

namespace spatacs { namespace game {


    class GameSimulation
            : public core::SimulationBase
    {
    public:
        GameSimulation();
        virtual ~GameSimulation() = default;

    private:
        void physics_callback(physics::PhysicsWorld& world, const physics::Object& A, const physics::Object& B,
                              physics::ImpactInfo info);

        void update() override;

        std::unique_ptr<physics::PhysicsWorld> mWorld;

        void eventLoop() override;
    };

}}


#endif //SPATACS_GAMESIMULATION_H
