//
// Created by erik on 10/17/16.
//

#ifndef SPATACS_COMPS_H
#define SPATACS_COMPS_H

#include "physics/units.h"
#include "core/Entity.h"
#include "game/Damage.h"
#include <boost/property_tree/ptree_fwd.hpp>

namespace spatacs
{
    namespace game
    {
        struct Name
        {
            Name(std::string n = "") : name(n) { }
            std::string name = "";
        };

        struct Health
        {
            Health() = default;
            Health(double v) : current(v), maximum(v) { }

            double status() const { return current / maximum; }

            double current = 0;
            double maximum = 0;
        };

        struct EnergyManagement
        {
            double cache        = 0;
            double last_request = 0;
            double priority     = 1;

            // helper functions
            double requestEnergy(double amount);
        };

        struct EngineData
        {
            EngineData() = default;
            EngineData(speed_t s, rate_t<mass_t> r) :
                mPropellantSpeed(s), mMassRate(r)
            {

            }

            // engine config
            speed_t mPropellantSpeed = 0.0_kps;
            rate_t<mass_t> mMassRate = 1.0_kg / 1.0_s;
        };

        struct FuelStorage
        {
            FuelStorage() = default;
            FuelStorage(mass_t v) : current(v), capacity(v) { }

            mass_t request(mass_t desire);

            // data about stored fuel
            mass_t current;
            mass_t capacity;
        };

        struct PowerPlantData
        {
            PowerPlantData() = default;
            PowerPlantData(double ep) : energy_production(ep) {}
            double energy_production = 0;
        };

        struct ShieldGeneratorData
        {
            ShieldGeneratorData() = default;

            // properties
            rate_t<scalar_t> mShieldRecharge{0.0};
            rate_t<scalar_t> mDecay{0.0};
            float mEnergyPerShieldPoint  = 5;
        };

        struct LifeSupportData
        {
            velocity_vec mLastVelocity{.0, .0, .0}; /// \todo this should probably come from somewhere else!
        };

        struct WeaponAimData
        {
            WeaponAimData() = default;
            WeaponAimData(speed_t speed, double prec) :
                muzzle_velocity(speed), precision(prec)
            {}

            speed_t muzzle_velocity{0};
            double precision = 1;
        };

        struct Timer
        {
            double time = 0;
        };

        struct ProjectileWpnData
        {
            enum Mode
            {
                AP_MODE,
                HE_MODE,
                SO_MODE
            } mMode = HE_MODE;
            Damage mDamage;
            float mRPM;
            mass_t mCaliber;

            Damage damage() const;
        };

        using ComponentEntity = core::Entity<Health, EnergyManagement, FuelStorage, EngineData,
                PowerPlantData, ShieldGeneratorData, LifeSupportData, WeaponAimData, ProjectileWpnData,
                        Timer, Name>;


        // creation functions
        void makeEngine(const boost::property_tree::ptree& data, ComponentEntity& cmp);
        void makeFuelTank(const boost::property_tree::ptree& data, ComponentEntity& cmp);
        void makeLifeSupport(const boost::property_tree::ptree& data, ComponentEntity& cmp);
        void makePowerPlant(const boost::property_tree::ptree& data, ComponentEntity& cmp);
        void makeProjectileWpn(const boost::property_tree::ptree& data, ComponentEntity& cmp);
        void makeShieldGenerator(const boost::property_tree::ptree& data, ComponentEntity& cmp);
    }
}

#endif //SPATACS_COMPS_H
