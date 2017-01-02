//
// Created by erik on 1/2/17.
//

#include "Spawn.h"
#include "game/State.h"
#include "game/entity/entity.h"
#include "physics/PhysicsWorld.h"

using namespace spatacs;
using namespace game;
using namespace events;

SpawnProjectile::SpawnProjectile(GameEntityID shooter, length_vec pos, velocity_vec vel, mass_t mass, length_t rad,
                                 game::Damage dmg) :
        mShooter(shooter),
        mPosition(pos),
        mVelocity(vel),
        mMass(mass),
        mRadius(rad),
        mDamage(dmg)
{

}

void SpawnProjectile::apply(EventContext& context) const
{
    GameEntityHandle entity = context.state.entity_manager().create();
    entity.add<components::Timer>(10.0_s);
    entity.add<components::Parent>(mShooter);
    entity.add<components::Warhead>(mDamage);

    physics::Object obj(mPosition, mVelocity, mMass, entity.id().getID());
    obj.addFixture( mRadius );
    auto pid = context.world.spawn(obj);

    entity.add<components::PhysicsData>(pid, mMass, mPosition, mVelocity);
}
