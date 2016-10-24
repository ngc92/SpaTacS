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

        Activity::Activity(double a) : mActivity(a)
        {

        }

        double Activity::get() const
        {
            return mActivity;
        }
        void Activity::set(double a)
        {
            mActivity= std::max(0.0, std::min(1.0, a));
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
            cmp.add<Activity>();
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
            cmp.add<Activity>();
        }

        void makeProjectileWpn(const ptree& data, ComponentEntity& cmp)
        {
            addHealth(cmp, data);
            cmp.add<WeaponAimData>(1.0_km / 1.0_s, data.get<double>("precision"));
            auto& pwd = cmp.add<ProjectileWpnData>();
            pwd.mRPM = data.get<float>("rpm");
            pwd.mAmmo = "SO-light";
            cmp.add<Timer>();
        }

        void makeShieldGenerator(const ptree& data, ComponentEntity& cmp)
        {
            addHealth(cmp, data);
            cmp.add<Name>("shield generator");
            auto& sgd = cmp.add<ShieldGeneratorData>();
            sgd.mShieldRecharge = data.get<rate_t<scalar_t>>("recharge");
            sgd.mEnergyPerShieldPoint = data.get<energy_t>("consumption");
            cmp.add<Activity>();
        }

        void makeAmmoStorage(const boost::property_tree::ptree& data, ComponentEntity& cmp)
        {
            std::size_t cap = data.get<std::size_t>("capacity");
            cmp.add<AmmoStorage>(cap);
        }
    }
}
