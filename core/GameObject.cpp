//
// Created by erik on 9/28/16.
//

#include <iostream>
#include "GameObject.h"
#include "Starship.h"

using namespace spatacs;
using namespace spatacs::core;

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

std::uint64_t GameObject::physics_id() const
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

void GameObject::setPhysicsID(std::uint64_t id)
{
    mPhysicsID = id;
}

void GameObject::setMass(mass_t m)
{
    mMass = m;
}
