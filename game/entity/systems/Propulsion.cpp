//
// Created by erik on 12/31/16.
//

#include "game/entity/entity_comps.h"
#include "physics/PhysicsWorld.h"
#include "Propulsion.h"
#include "game/entity/subs/systems/Propulsion.h"

#include "core/ecs/EntityManager.h"
#include "game/entity/subs/subcomponents.h"

using namespace spatacs;
using namespace game;

void systems::ApplyPropulsion::operator()(PhysicsData& pd, PropulsionControl& pc, SubEntities& se) const
{
    /// \todo
    /// we need to get the force from somewhere. For Starships and other complex objects, this
    /// would be the subcomponents, for fighters, rockets and missiles we probably want something easier.
    /// For now we just add the subcomponent to the signature, and let that work.

    auto desired_force = pc.desired_acceleration() * pd.mass();

    EngineThrust thrust;
    se.submgr().apply(thrust, se.fuelmgr(), desired_force);

    pc.setMaximumAcceleration( thrust.getMax() / pd.mass() );
    pd.setForce( pd.force() + thrust.getProduced() );
}
