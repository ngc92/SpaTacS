//
// Created by erik on 9/21/16.
//

#include "components.h"
#include <boost/property_tree/ptree.hpp>

namespace spatacs
{
    namespace game
    {
        using boost::property_tree::ptree;

        //
        // helper functions
        double EnergyManagement::requestEnergy(double amount)
        {
            last_request += amount;
            if (amount < cache) {
                cache -= amount;
                return amount;
            }
            double ret = cache;
            cache = 0;
            return ret;
        }

        Damage ProjectileWpnData::damage() const
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

        mass_t FuelStorage::request(mass_t desire)
        {
            desire = std::min(desire, current);
            current -= desire;
            return desire;
        }

        // creation functions
        void addHealth(ComponentEntity& cmp, const ptree& data)
        {
            cmp.add<Health>( data.get<double>("HP") );
        }

        void makeEngine(const ptree& data, ComponentEntity& cmp)
        {
            addHealth(cmp, data);
            cmp.add<Name>("engine");
            cmp.add<EngineData>(data.get<speed_t>("propellant_speed"),
                                data.get<physics::rate_t<mass_t>>("fuel_consumption") );
        }

        void makeFuelTank(const ptree& data, ComponentEntity& cmp)
        {
            addHealth(cmp, data);
            cmp.add<FuelStorage>(data.get<mass_t>("capacity") );
        }

        void makeLifeSupport(const ptree& data, ComponentEntity& cmp)
        {
            addHealth(cmp, data);
            cmp.add<EnergyManagement>();
            cmp.add<LifeSupportData>();
        }

        void makePowerPlant(const ptree& data, ComponentEntity& cmp)
        {
            addHealth(cmp, data);
            cmp.add<EnergyManagement>();
            cmp.add<PowerPlantData>(data.get<double>("power") );
        }

        void makeProjectileWpn(const ptree& data, ComponentEntity& cmp)
        {
            addHealth(cmp, data);
            cmp.add<WeaponAimData>(data.get<speed_t>("muzzle_velocity"), data.get<double>("precision"));
            auto& pwd = cmp.add<ProjectileWpnData>();
            pwd.mDamage = Damage{ data.get<float>("HE_strength"),
                                  data.get<float>("SO_strength"),
                                  data.get<float>("AP_strength") };
            pwd.mRPM = data.get<float>("rpm");
            pwd.mCaliber = data.get<mass_t>("caliber");
            cmp.add<Timer>();
        }

        void makeShieldGenerator(const ptree& data, ComponentEntity& cmp)
        {
            addHealth(cmp, data);
            cmp.add<Name>("shield generator");
            cmp.add<EnergyManagement>();
            auto& sgd = cmp.add<ShieldGeneratorData>();
            sgd.mShieldRecharge = scalar_t(data.get<float>("recharge")) / 1.0_s;
            sgd.mDecay =  scalar_t(std::log(1-data.get<float>("dissipation") / 100.f)) / 1.0_s;
            sgd.mEnergyPerShieldPoint = 1.f / data.get<float>("efficiency");
        }
    }
}
