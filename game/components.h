//
// Created by erik on 10/17/16.
//

#ifndef SPATACS_COMPS_H
#define SPATACS_COMPS_H

#include "physics/units.h"
#include "game/Damage.h"
#include <boost/property_tree/ptree_fwd.hpp>
#include <vector>
#include "core/ecs/fwd.h"
#include "ObjectID.h"
#include "systems/fwd.h"

namespace spatacs
{
    namespace game
    {
        struct Name
        {
            explicit Name(std::string n = "") : name(n) { }
            std::string name = "";
        };

        struct Health
        {
            Health() = default;
            explicit Health(double v) : current(v), maximum(v) { }

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
        };

        struct FuelStorage
        {
            FuelStorage() = default;
            explicit FuelStorage(mass_t v);

            mass_t request(mass_t desire);
            mass_t fill(mass_t amount);

            // data about stored fuel
            mass_t current;
            mass_t capacity;
        };

        struct AmmoData
        {
            std::string name;
            mass_t   mass;
            energy_t charge;
            Damage   damage;
        };

        struct AmmoStorage
        {
            explicit AmmoStorage(std::size_t cap = 0);

            struct Ammo
            {
                std::size_t amount;
                AmmoData    data;
            };

            /// changes the ammount of \p a by the amount added.
            void addAmmo(const AmmoData& data, std::size_t& amount);
            Ammo& getAmmo(const std::string& type);

            std::vector<Ammo> ammo;
            std::size_t capacity = 0;
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

        class Activity
        {
        public:
            explicit Activity(double a = 1);

            double get() const;
            void set(double a);
        private:
            double mActivity;
        };

        // creation functions
        void makeEngine(const boost::property_tree::ptree& data, SubsystemHandle cmp);
        void makeFuelTank(const boost::property_tree::ptree& data, SubsystemHandle cmp);
        void makeLifeSupport(const boost::property_tree::ptree& data, SubsystemHandle cmp);
        void makePowerPlant(const boost::property_tree::ptree& data, SubsystemHandle cmp);
        void makeProjectileWpn(const boost::property_tree::ptree& data, SubsystemHandle cmp);
        void makeShieldGenerator(const boost::property_tree::ptree& data, SubsystemHandle cmp);
        void makeAmmoStorage(const boost::property_tree::ptree& data, SubsystemHandle cmp);
    }
}

#endif //SPATACS_COMPS_H
