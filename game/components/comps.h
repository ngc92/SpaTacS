//
// Created by erik on 10/17/16.
//

#ifndef SPATACS_COMPS_H
#define SPATACS_COMPS_H

#include "physics/units.h"
#include "core/Entity.h"
#include "game/Damage.h"

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
            double mEnergyCache      = 0;
            double mLastTotalRequest = 0;
            double mEnergyPriority   = 1;

            // helper functions
            double requestEnergy(double amount)
            {
                mLastTotalRequest += amount;
                if (amount < mEnergyCache) {
                    mEnergyCache -= amount;
                    return amount;
                }
                double ret = mEnergyCache;
                mEnergyCache = 0;
                return ret;
            }
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

            // data about stored fuel
            mass_t current;
            mass_t capacity;
        };

        struct PowerPlantData
        {
            PowerPlantData() = default;
            PowerPlantData(double ep) : mEnergyProduction(ep) {}
            double mEnergyProduction = 0;
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
            velocity_vec mLastVelocity; /// \todo this should probably come from somewhere else!
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

            Damage damage() const
            {
                Damage dmg;
                if (mMode == HE_MODE) {
                    dmg.high_explosive = mDamage.high_explosive;
                } else if (mMode == ProjectileWpnData::AP_MODE) {
                    dmg.armour_piercing = mDamage.armour_piercing;
                } else if (mMode == ProjectileWpnData::SO_MODE) {
                    dmg.shield_overload = mDamage.shield_overload;
                }

                return dmg;
            }
        };

        using ComponentEntity = core::Entity<Health, EnergyManagement, FuelStorage, EngineData,
                PowerPlantData, ShieldGeneratorData, LifeSupportData, WeaponAimData, ProjectileWpnData,
                        Timer, Name>;
    }
}

#endif //SPATACS_COMPS_H
