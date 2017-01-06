//
// Created by erik on 1/3/17.
//

#ifndef SPATACS_GAME_ENTITY_SUB_PROPULSION_H
#define SPATACS_GAME_ENTITY_SUB_PROPULSION_H

#include "../fwd.h"
#include "physics/units.h"

namespace spatacs
{
namespace game
{
namespace systems
{
    using namespace components;

    class EngineThrust : public System<signatures::Engine>
    {
    public:
        EngineThrust() = default;
        void operator()(EngineData& engine, const Health& health, const Activity& acc,
                        FuelDistributor& fuelmgr,
                        const force_vec& desired_force);

        const force_vec& getProduced() const { return mProducedForce; }
        force_t getMax() const { return mMaxForce; }

    private:
        force_vec mProducedForce{.0, .0, .0};
        force_t   mMaxForce{.0};
    };
}
}
}

#endif // SPATACS_GAME_ENTITY_SUB_PROPULSION_H