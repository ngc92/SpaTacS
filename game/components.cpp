//
// Created by erik on 9/21/16.
//

#include "components.h"
#include "systems.h"
#include <boost/property_tree/ptree.hpp>

namespace spatacs
{
    namespace game
    {
        using boost::property_tree::ptree;

        mass_t FuelStorage::request(mass_t desire)
        {
            desire = std::min(desire, current);
            current -= desire;
            return desire;
        }

        AmmoStorage::AmmoStorage(std::size_t cap) : capacity(cap)
        {

        }

        void AmmoStorage::addAmmo(Ammo& a)
        {
            std::size_t used = 0;
            for(auto& am : ammo)
                used += am.amount;
            if(used > capacity)
                return;
            if(capacity - used > a.amount) {
                ammo.push_back(a);
                a.amount = 0;
            } else
            {
                ammo.push_back(a);
                ammo.back().amount = capacity - used;
                a.amount -= ammo.back().amount;
            }

        }

        AmmoStorage::Ammo& AmmoStorage::getAmmo(const std::string& type)
        {
            static Ammo empty;
            empty.amount = 0;
            for(auto& a : ammo)
            {
                if(a.name == type)
                    return a;
            }
            return empty;
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
            cmp.add<FuelStorage>(data.get<mass_t>("capacity") );
        }

        void makeLifeSupport(const ptree& data, ComponentEntity& cmp)
        {
            addHealth(cmp, data);
            cmp.add<LifeSupportData>();
        }

        void makePowerPlant(const ptree& data, ComponentEntity& cmp)
        {
            addHealth(cmp, data);
            cmp.add<PowerPlantData>(data.get<power_t>("power") );
        }

        void makeProjectileWpn(const ptree& data, ComponentEntity& cmp)
        {
            addHealth(cmp, data);
            cmp.add<WeaponAimData>(1.0_km / 1.0_s, data.get<double>("precision"));
            auto& pwd = cmp.add<ProjectileWpnData>();
            pwd.mRPM = data.get<float>("rpm");
            pwd.mAmmo = "SO";
            cmp.add<Timer>();
        }

        void makeShieldGenerator(const ptree& data, ComponentEntity& cmp)
        {
            addHealth(cmp, data);
            cmp.add<Name>("shield generator");
            auto& sgd = cmp.add<ShieldGeneratorData>();
            sgd.mShieldRecharge = data.get<rate_t<scalar_t>>("recharge");
            sgd.mEnergyPerShieldPoint = data.get<energy_t>("consumption");
        }

        void makeAmmoStorage(const boost::property_tree::ptree& data, ComponentEntity& cmp)
        {
            std::size_t cap = data.get<std::size_t>("capacity");
            auto& as = cmp.add<AmmoStorage>(cap);
            for(auto am : data)
            {
                if(am.second.count("amount") != 0) {
                    std::string name = am.first;
                    std::size_t amount = am.second.get<std::size_t>("amount");
                    mass_t mass = am.second.get<mass_t>("mass");
                    energy_t energy = am.second.get<energy_t>("energy");
                    Damage dmg;
                    dmg.armour_pierce   = am.second.get<double>("AP", 0.0);
                    dmg.high_explosive  = am.second.get<double>("HE", 0.0);
                    dmg.shield_overload = am.second.get<double>("SO", 0.0);
                    AmmoStorage::AmmoData ad{mass, energy, dmg};
                    AmmoStorage::Ammo st{name, amount, ad};
                    as.addAmmo(st);
                }
            }
        }
    }
}
