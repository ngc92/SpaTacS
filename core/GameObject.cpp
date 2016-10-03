//
// Created by erik on 9/28/16.
//

#include <iostream>
#include "GameObject.h"

using namespace spatacs;
using namespace spatacs::core;

const length_vec& GameObject::position() const
{
    return mPhysicsObj.position();
}

const velocity_vec& GameObject::velocity() const
{
    return mPhysicsObj.velocity();
}

length_t GameObject::radius() const
{
    return mPhysicsObj.radius();
}

std::uint64_t GameObject::id() const
{
    return mPhysicsObj.userdata();
}

std::uint64_t GameObject::physics_id() const
{
    return mPhysicsObj.id();
}

void GameObject::setID(std::uint64_t id)
{
    mPhysicsObj.setUserdata(id);
}

physics::Object& GameObject::getPhysicsObject()
{
    return mPhysicsObj;
}

mass_t GameObject::mass() const
{
    return mPhysicsObj.mass();
}
