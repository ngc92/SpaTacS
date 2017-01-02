//
// Created by erik on 12/31/16.
//

#ifndef SPATACS_GAME_ENTITY_ENTITY_COMPONENTS_H
#define SPATACS_GAME_ENTITY_ENTITY_COMPONENTS_H

#include "physics/Object.h"
#include "fwd.h"
#include "common_comps.h"

namespace spatacs
{
namespace game
{
    class Damage;
namespace components
{
    struct PhysicsData
    {
        PhysicsData() = default;
        PhysicsData(physics::ObjectID id, mass_t mass, length_vec pos, velocity_vec vel) :
            mPhysicsID(id), mMass(mass), mPosition(pos), mVelocity(vel)
        {}

        physics::ObjectID mPhysicsID;
        mass_t            mMass;

        length_vec   mPosition;
        velocity_vec mVelocity;
    };

    struct Parent
    {
        Parent() = default;
        Parent(GameEntityID id) : mParent(id) { }
        GameEntityID mParent;
    };

    struct Warhead
    {
        Warhead() = default;
        Warhead(const Damage& dmg) { }
    };

    struct Affiliation
    {
        std::uint64_t mTeam;
    };

    struct Armour
    {

    };

    struct PropulsionControl
    {
        force_vec produced_force;
    };

    // propulsion control
    // energy mgm?
}
}
}

#endif //SPATACS_GAME_ENTITY_ENTITY_COMPONENTS_H
