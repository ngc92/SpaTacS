//
// Created by erik on 9/28/16.
//

#include <iostream>
#include "game/GameObject.h"
#include "game/Starship.h"

using namespace spatacs;
using namespace spatacs::game;

const length_vec& GameObject::position() const
{
    return mPosition;
}

const velocity_vec& GameObject::velocity() const
{
    return mVelocity;
}

std::uint64_t GameObject::id() const
{
    return mID;
}

physics::ObjectID GameObject::physics_id() const
{
    return mPhysicsID;
}

void GameObject::setID(std::uint64_t id)
{
    mID = id;
}

mass_t GameObject::mass() const
{
    return mMass;
}

void GameObject::setPosition(const length_vec& pos)
{
    mPosition = pos;
}

void GameObject::setVelocity(const velocity_vec& vel)
{
    mVelocity = vel;
}

void GameObject::setPhysicsID(physics::ObjectID id)
{
    mPhysicsID = id;
}

void GameObject::setMass(mass_t m)
{
    mMass = m;
}

GameObject::GameObject(ObjectType t) : mType(t)
{

}

ObjectType GameObject::type() const
{
    return mType;
}

length_t spatacs::game::distance(const GameObject& s1, const GameObject& s2)
{
    return length(s1.position() - s2.position());
}