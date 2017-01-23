//
// Created by erik on 12/31/16.
//

#ifndef SPATACS_GAME_ENTITY_ENTITY_COMPONENTS_H
#define SPATACS_GAME_ENTITY_ENTITY_COMPONENTS_H

#include <memory>
#include "physics/Object.h"
#include "fwd.h"

namespace spatacs
{
namespace game
{
    class Damage;
namespace components
{
    /*! Component that contains a textual identifier for the entity. The identifier
     * is intended to be used for user interaction. It might be translated, so never act
     * depending on the value here. Deliberately does not provide a way to change the name
     * after construction.
     */
    struct Name
    {
    public:
        explicit Name(std::string n = "") : mName(std::move(n)) {}

        const std::string& name() const { return mName; }

    private:
        std::string mName = "";
    };

    /*! A generic health component that saves a current and maximum health value.
     *  Provides some utility functions for getting/setting health and status.
     *  Invariant: 0 <= current <= maximum.
     */
    struct Health
    {
    public:
        explicit Health(double v = 0.0) : mCurrent(v), mMaximum(v) {}

        double current() const { return mCurrent; }
        double maximum() const { return mMaximum; }
        double status() const { return mCurrent / mMaximum; }

        /// sets the current health to \p new_health, clamped between 0 and maximum().
        /// \return the new health value.
        double setCurrent(double new_health);

        /// sets the maximum health. Clamps the current health to the new range.
        /// \throw std::logic_error, if \p new_maximum < 0.
        void setMaximum(double new_maximum);

        /// sets the health relative to maximum health.
        /// \return The new current health.
        double setHealthStatus(double percentage);

    private:
        double mCurrent = 0;
        double mMaximum = 0;
    };

    struct Timer
    {
        Timer() = default;

        explicit Timer(time_t t) : time(t) {}

        time_t time = 0.0_s;
    };

    class Activity
    {
    public:
        explicit Activity(double a = 1);

        double get() const;

        void set(double a);

    private:
        double mActivity;
    };

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

    struct Engine
    {
        Engine() = default;
        Engine(speed_t s, rate_t<mass_t> r) :
                propellant_speed(s), mass_rate(r)
        {

        }

        // engine config
        speed_t propellant_speed = 0.0_kps;
        rate_t<mass_t> mass_rate = 1.0_kg / 1.0_s;
    };

    class FuelTank
    {
    public:
        FuelTank() = default;
        explicit FuelTank(mass_t v);

        mass_t request(mass_t desire);
        mass_t fill(mass_t amount);
        mass_t capacity() const;
    private:
        // data about stored fuel
        mass_t mCurrent{0};
        mass_t mCapacity{0};
    };

    struct PowerPlantData
    {
        PowerPlantData() = default;
        explicit PowerPlantData(power_t ep) : energy_production(ep) {}
        power_t energy_production = 0.0_W;
    };

    struct ShieldGeneratorData
    {
        ShieldGeneratorData() = default;

        // properties
        rate_t<scalar_t> mShieldRecharge{0.0};
        energy_t         mEnergyPerShieldPoint{1.0};
    };

    // energy mgm?
}
}
}

#endif //SPATACS_GAME_ENTITY_ENTITY_COMPONENTS_H
