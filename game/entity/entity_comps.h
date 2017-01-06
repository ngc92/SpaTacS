//
// Created by erik on 12/31/16.
//

#ifndef SPATACS_GAME_ENTITY_ENTITY_COMPONENTS_H
#define SPATACS_GAME_ENTITY_ENTITY_COMPONENTS_H

#include <memory>
#include "physics/Object.h"
#include "fwd.h"
#include "common_comps.h"
#include "subs/fwd.h"

namespace spatacs
{
namespace game
{
    class Damage;
namespace components
{
    class PhysicsData
    {
    public:
        PhysicsData() = default;
        PhysicsData(physics::ObjectID id, mass_t mass, length_vec pos, velocity_vec vel);

        physics::ObjectID id() const;

        PhysicsData& setPosition(const length_vec& pos);
        PhysicsData& setVelocity(const velocity_vec& vel);
        PhysicsData& setForce(const force_vec& force);
        PhysicsData& setMass(mass_t m);

        mass_t mass() const;
        const length_vec&   position() const;
        const velocity_vec& velocity() const;
        const force_vec&    force() const;
    private:
        physics::ObjectID mPhysicsID;
        mass_t            mMass;

        length_vec   mPosition;
        velocity_vec mVelocity;

        // dynamics
        force_vec mForce;
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
        Affiliation() = default;
        Affiliation(std::uint64_t team) : mTeam(team) { }
        std::uint64_t mTeam;
    };

    struct Armour
    {

    };

    class PropulsionControl
    {
    public:
        PropulsionControl() = default;

        void setDesiredAcceleration(const accel_vec& desired);
        const accel_vec& desired_acceleration() const;

        void setMaximumAcceleration(accel_t macc);
        accel_t maximum_acceleration() const;
    private:
        accel_vec mDesiredAcceleration{0,0,0};     //!< Acceleration that is desired by the higher level control unit.
        accel_t   mMaximumAcceleration{0};         //!< Estimated maximum possible propulsion.
    };

    class EnergyManager
    {
    public:
    private:

    };

    class SubEntities
    {
    public:
        SubEntities();
        ~SubEntities();
        explicit SubEntities(const SubEntities&);
        SubEntities(SubEntities&&);

        SubEntityManager& submgr();
        const SubEntityManager& submgr() const;

        FuelDistributor& fuelmgr();
        const FuelDistributor& fuelmgr() const;
    private:
        class Data;
        std::unique_ptr<Data> mData;
    };

    // energy mgm?
}
}
}

#endif //SPATACS_GAME_ENTITY_ENTITY_COMPONENTS_H
