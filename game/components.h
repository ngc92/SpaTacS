//
// Created by erik on 10/17/16.
//

#ifndef SPATACS_COMPS_H
#define SPATACS_COMPS_H

#include "physics/units.h"
#include "core/Entity.h"
#include "game/Damage.h"
#include <boost/property_tree/ptree_fwd.hpp>
#include <vector>

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

        struct EngineData
        {
            EngineData() = default;
            EngineData(speed_t s, rate_t<mass_t> r) :
                propellant_speed(s), mass_rate(r)
            {

            }

            // engine config
            speed_t propellant_speed = 0.0_kps;
            rate_t<mass_t> mass_rate = 1.0_kg / 1.0_s;

            std::uint64_t fuel_source;
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

        struct AmmoStorage
        {
            AmmoStorage(std::size_t cap = 0);

            struct AmmoData
            {
                mass_t      mass;
                energy_t    charge;
                Damage      damage;
            };

            struct Ammo
            {
                std::string name;
                std::size_t amount;
                AmmoData    data;
            };

            /// changes the ammount of \p a by the amount added.
            void addAmmo(Ammo& a);
            Ammo& getAmmo(const std::string& type);

            std::vector<Ammo> ammo;
            std::size_t capacity = 0;
        };

        struct PowerPlantData
        {
            PowerPlantData() = default;
            PowerPlantData(power_t ep) : energy_production(ep) {}
            power_t energy_production = 0.0_W;
        };

        struct ShieldGeneratorData
        {
            ShieldGeneratorData() = default;

            // properties
            rate_t<scalar_t> mShieldRecharge{0.0};
            energy_t         mEnergyPerShieldPoint{1.0};
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
            std::string mAmmo;
            float mRPM;
        };

        using ComponentEntity = core::Entity<Health, FuelStorage, EngineData,
                PowerPlantData, ShieldGeneratorData, LifeSupportData, WeaponAimData, ProjectileWpnData,
                        Timer, Name, AmmoStorage>;


        // creation functions
        void makeEngine(const boost::property_tree::ptree& data, ComponentEntity& cmp);
        void makeFuelTank(const boost::property_tree::ptree& data, ComponentEntity& cmp);
        void makeLifeSupport(const boost::property_tree::ptree& data, ComponentEntity& cmp);
        void makePowerPlant(const boost::property_tree::ptree& data, ComponentEntity& cmp);
        void makeProjectileWpn(const boost::property_tree::ptree& data, ComponentEntity& cmp);
        void makeShieldGenerator(const boost::property_tree::ptree& data, ComponentEntity& cmp);
        void makeAmmoStorage(const boost::property_tree::ptree& data, ComponentEntity& cmp);
    }
}

#endif //SPATACS_COMPS_H
