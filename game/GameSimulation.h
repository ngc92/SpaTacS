//
// Created by erik on 11/15/16.
//

#ifndef SPATACS_GAMESIMULATION_H
#define SPATACS_GAMESIMULATION_H

#include "core/SimulationBase.h"
#include "physics/PhysicsWorld.h"
#include "GameState.h"
#include "events/events.h"
#include <queue>

namespace spatacs { namespace game {


    class GameSimulation
            : public core::SimulationBase
    {
    public:
        GameSimulation();
        virtual ~GameSimulation() = default;

        void processInput(EventVec inEvents) override;
        NotifyVec update() override;

    private:

        void physics_callback(physics::PhysicsWorld& world, const physics::Object& A, const physics::Object& B,
                              physics::ImpactInfo info);

        std::unique_ptr<physics::PhysicsWorld> mWorld;

        void eventLoop();

        const GameState& getState() const override;

        using EventQueue = std::deque<EventPtr>;
        EventQueue mEventQueue;
        std::vector<events::notification_t> mNotifications;

        // thread internal variables, don't touch from outside!
        std::unique_ptr<GameState> mState;
    };

}}


#endif //SPATACS_GAMESIMULATION_H
