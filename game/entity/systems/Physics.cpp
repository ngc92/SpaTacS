//
// Created by erik on 1/3/17.
//

#include "game/entity/components.h"
#include "physics/PhysicsWorld.h"
#include "Physics.h"

using namespace spatacs;
using namespace game;
using namespace systems;

void UpdateFromWorld::operator()(PhysicsData& pd, const physics::PhysicsWorld& pw) const
{
    auto po = pw.getObject(pd.id());
    pd.setPosition(po.position());
    pd.setVelocity(po.velocity());
    pd.setMass(po.mass());
    pd.setForce( force_vec{0, 0, 0} );
}

void UpdateToWorld::operator()(const PhysicsData& pd, physics::PhysicsWorld& pw) const
{
    pw.updateObject(pd.id(), pd.position(), pd.velocity(), pd.mass());
    if(pd.force() != force_vec{0,0,0})
    {
        pw.applyForce(pd.id(), pd.force());
    }
}