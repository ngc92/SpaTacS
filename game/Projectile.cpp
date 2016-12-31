//
// Created by erik on 9/20/16.
//

#include <limits>
#include <iostream>
#include "game/Projectile.h"

using namespace spatacs::game;
using spatacs::Vec;

Projectile::Projectile(ObjectID shooter, Damage damage) :
    GameObject(ObjectType::PROJECTILE),
    mShooterId(shooter),
    mDamage( damage )
{
}

void Projectile::onStep(GameState& state)
{
    mAge += 0.1;
}

float Projectile::age() const
{
    return mAge;
}

ObjectID Projectile::shooter() const
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

