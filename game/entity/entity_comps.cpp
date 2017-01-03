//
// Created by erik on 1/3/17.
//

#include "entity_comps.h"
#include "subs/fwd.h"
#include "core/ecs/EntityManager.h"
#include "subs/subcomponents.h"

using namespace spatacs;
using namespace game;
using namespace components;

PhysicsData::PhysicsData(physics::ObjectID id, mass_t mass, length_vec pos, velocity_vec vel) :
    mPhysicsID(id), mMass(mass), mPosition(pos), mVelocity(vel)
{

}

PhysicsData& PhysicsData::setPosition(const length_vec& pos)
{
    mPosition = pos;
    return *this;
}

PhysicsData& PhysicsData::setVelocity(const velocity_vec& vel)
{
    mVelocity = vel;
    return *this;
}

PhysicsData& PhysicsData::setMass(mass_t m)
{
    mMass = m;
    return *this;
}

physics::ObjectID PhysicsData::id() const
{
    return mPhysicsID;
}

PhysicsData& PhysicsData::setForce(const force_vec& force)
{
    mForce = force;
    return *this;
}

mass_t PhysicsData::mass() const
{
    return mMass;
}

const length_vec& PhysicsData::position() const
{
    return mPosition;
}

const velocity_vec& PhysicsData::velocity() const
{
    return mVelocity;
}

const force_vec& PhysicsData::force() const
{
    return mForce;
}

// ---------------------------------------------------------------------------------------------------------------------
//      Sub Entities stuff
// ---------------------------------------------------------------------------------------------------------------------
struct SubEntities::Data
{
    SubEntityManager mgr;
};

SubEntities::SubEntities() : mData( std::make_unique<Data>() )
{

}

SubEntities::SubEntities(const SubEntities& o) : mData( std::make_unique<Data>( *o.mData ) )
{

}

SubEntities::SubEntities(SubEntities&&) = default;