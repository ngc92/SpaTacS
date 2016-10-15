//
// Created by erik on 9/20/16.
//

#include <limits>
#include <iostream>
#include "Projectile.h"

using namespace spatacs::core;
using spatacs::Vec;

Projectile::Projectile(std::uint64_t id, std::uint64_t shooter, Damage damage) :
    GameObject(ObjectType::PROJECTILE),
    mDamage( damage ), mShooterId(shooter)
{
    setID(id);
}

void Projectile::onStep()
{
    mAge += 0.1;
}

float Projectile::age() const
{
    return mAge;
}

std::uint64_t Projectile::shooter() const
{
    return mShooterId;
}

void Projectile::expire()
{
    // don't use max, because then adding time will cause overflow
    mAge = std::numeric_limits<float>::max() / 2;
}

const Damage& Projectile::damage() const
{
    return mDamage;
}

bool Projectile::alive() const
{
    return mAge < 10;
}

Projectile* Projectile::clone() const
{
    return new Projectile(*this);
}

