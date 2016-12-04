//
// Created by erik on 9/21/16.
//

#include "components.h"
#include <boost/property_tree/ptree.hpp>
#include "core/ecs/EntityHandle.h"
#include "core/ecs/EntityManager.h"

namespace spatacs
{
    namespace game
    {
        using boost::property_tree::ptree;

        FuelStorage::FuelStorage(mass_t v) : current(0.0_kg), capacity(v) { }

        mass_t FuelStorage::request(mass_t desire)
        {
            desire = std::min(desire, current);
            current -= desire;
            return desire;
        }

        mass_t FuelStorage::fill(mass_t amount)
        {
            mass_t free = capacity - current;
            mass_t add  = std::min(free, amount);
            amount -= add;
            current += add;
            return amount;
        }

        AmmoStorage::AmmoStorage(std::size_t cap) : capacity(cap)
        {

        }

        void AmmoStorage::addAmmo(const AmmoData& data, std::size_t& amount)
        {
            std::size_t used = 0;
            for(auto& am : ammo)
                used += am.amount;
            if(used > capacity)
                return;

            if(capacity - used > amount) {
                ammo.push_back(Ammo{amount, data});
                amount = 0;
            } else
            {
                ammo.push_back(Ammo{amount, data});
                ammo.back().amount = capacity - used;
                amount -= ammo.back().amount;
            }

        }

        AmmoStorage::Ammo& AmmoStorage::getAmmo(const std::string& type)
        {
            static Ammo empty;
            empty.amount = 0;
            for(auto& a : ammo)
            {
                if(a.data.name == type)
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
        void addHealth(SubsystemHandle& cmp, const ptree& data)
        {
            cmp.add<Health>( data.get<double>("HP") );
        }

        void makeEngine(const ptree& data, SubsystemHandle cmp)
        {
            addHealth(cmp, data);
            cmp.add<Name>("engine");
            cmp.add<EngineData>(data.get<speed_t>("propellant_speed"),
                                data.get<physics::rate_t<mass_t>>("fuel_consumption") );
            cmp.add<Activity>();
        }

        void makeFuelTank(const ptree& data, SubsystemHandle cmp)
        {
            cmp.add<FuelStorage>(data.get<mass_t>("capacity") );
        }

        void makeLifeSupport(const ptree& data, SubsystemHandle cmp)
        {
            addHealth(cmp, data);
            cmp.add<LifeSupportData>();
        }

        void makePowerPlant(const ptree& data, SubsystemHandle cmp)
        {
            addHealth(cmp, data);
            cmp.add<Name>("power plant");
            cmp.add<PowerPlantData>(data.get<power_t>("power") );
            cmp.add<Activity>();
        }

        void makeProjectileWpn(const ptree& data, SubsystemHandle cmp)
        {
            addHealth(cmp, data);
            cmp.add<WeaponAimData>(1.0_km / 1.0_s, data.get<double>("precision"));
            auto& pwd = cmp.add<ProjectileWpnData>();
            pwd.mRPM = data.get<float>("rpm");
            pwd.mAmmo = "SO-light";
            cmp.add<Timer>();
        }

        void makeShieldGenerator(const ptree& data, SubsystemHandle cmp)
        {
            addHealth(cmp, data);
            cmp.add<Name>("shield generator");
            auto& sgd = cmp.add<ShieldGeneratorData>();
            sgd.mShieldRecharge = data.get<rate_t<scalar_t>>("recharge");
            sgd.mEnergyPerShieldPoint = data.get<energy_t>("consumption");
            cmp.add<Activity>();
        }

        void makeAmmoStorage(const boost::property_tree::ptree& data, SubsystemHandle cmp)
        {
            std::size_t cap = data.get<std::size_t>("capacity");
            cmp.add<AmmoStorage>(cap);
        }
    }
}
