//
// Created by erik on 12/31/16.
//

#include "game/entity/entity_comps.h"
#include "physics/PhysicsWorld.h"
#include "Propulsion.h"

using namespace spatacs;
using namespace game;

void systems::ApplyPropulsion::operator()(const PhysicsData& pd, const PropulsionControl& pc,
                                                physics::PhysicsWorld& world) const
{
    world.applyForce(pd.mPhysicsID, pc.produced_force);
}
