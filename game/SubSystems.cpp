//
// Created by erik on 10/7/16.
//

#include "Starship.h"
#include "GameObject.h"
#include "game/SubSystems.h"
#include <boost/property_tree/ptree.hpp>
#include <iostream>
#include "game/components/ProjectileWeapon.h"
#include "core/System.h"

using namespace spatacs;
using namespace game;

namespace
{
    class TimerCountdown : public core::System<ComponentEntity, TimerCountdown, core::Signature<Timer>>
    {
    public:
        TimerCountdown(double dt) : mDeltaT(dt) { }

        void apply(const ComponentEntity& ety, Timer& timer) const
        {
            timer.time -= mDeltaT;
        }
    private:
        double mDeltaT;
    };

    // -----------------------------------------------------------------------------------

    class GetProvidedEnergy : public core::System<ComponentEntity, GetProvidedEnergy,
                                                  core::Signature<EnergyManagement>>
    {
    public:
        GetProvidedEnergy() : mCollectedEnergy(0) { }

        void apply(const ComponentEntity& ety, EnergyManagement& egy)
        {
            mCollectedEnergy += egy.mEnergyCache;
            egy.mEnergyCache = 0;
        }

        double energy() const { return mCollectedEnergy; }
    private:
        double mCollectedEnergy;
    };

    class CollectRequests : public core::System<ComponentEntity, CollectRequests,
            core::Signature<EnergyManagement>>
    {
    public:
        void apply(const ComponentEntity& ety, EnergyManagement& egy)
        {
            mRequests.push_back( egy.mLastTotalRequest );
            mPriority.push_back( egy.mEnergyPriority );
        }

        const std::vector<double>& requests() { return mRequests; }
        const std::vector<double>& priority() { return mPriority; }

    private:
        std::vector<double> mRequests;
        std::vector<double> mPriority;
    };

    class ProvideEnergy : public core::System<ComponentEntity, ProvideEnergy,
            core::Signature<EnergyManagement>>
    {
    public:
        ProvideEnergy(std::vector<double>& s) : mSupply(s) { }

        void apply(const ComponentEntity& ety, EnergyManagement& egy)
        {
            egy.mEnergyCache = mSupply.at(i);
            ++i;
        }

    private:
        unsigned i = 0;
        std::vector<double>& mSupply;
    };

    // -----------------------------------------------------------------------------------------------
    class ShieldManagement : public core::System<ComponentEntity, ShieldManagement,
            core::Signature<ShieldGeneratorData, EnergyManagement, Health>>
    {
    public:
        ShieldManagement(Starship& s) : ship(s) { }

        void apply(const ComponentEntity& ety, ShieldGeneratorData& sgen, EnergyManagement& egy, const Health& health)
        {
            auto dt = 0.1_s;
            // shield decay
            double decay = std::exp( sgen.mDecay*dt );
            auto shield = ship.shield();
            shield *= decay;
            if(shield < ship.max_shield())
            {
                double difference = ship.max_shield() - ship.shield();
                double recharge = sgen.mShieldRecharge * dt * health.status();
                double rec = std::min( recharge, difference );
                double consumption = rec * sgen.mEnergyPerShieldPoint;
                if(consumption != 0)
                    rec *= egy.requestEnergy( consumption ) / consumption;
                shield += rec;
            }
            ship.setShield( shield );
        }
    private:
        Starship& ship;
    };

    class LifeSupportStep : public core::System<ComponentEntity, LifeSupportStep,
            core::Signature<LifeSupportData, EnergyManagement>>
    {
    public:
        LifeSupportStep(const Starship& s) : ship(s) { }

        void apply(const ComponentEntity& ety, LifeSupportData& sup, EnergyManagement& egy)
        {
            auto accel = length(ship.velocity() - sup.mLastVelocity) / 0.1_s + 9.81_m/(1.0_s)/(1.0_s);
            sup.mLastVelocity = ship.velocity();
            double ereq = accel.value / 1000.0;
            /// \todo for now, nothing happens when we do not get the requested energy!
            egy.requestEnergy( ereq );
        }
    private:
        const Starship& ship;
    };

    class PowerProduction : public core::System<ComponentEntity, PowerProduction,
            core::Signature<PowerPlantData, EnergyManagement, Health>>
    {
    public:
        void apply(const ComponentEntity& ety, PowerPlantData& pp, EnergyManagement& egy, const Health& health)
        {
            egy.mEnergyCache += 0.1f * pp.mEnergyProduction * health.status();;
        }
    };

    class Propulsion : public core::System<ComponentEntity, Propulsion,
            core::Signature<EngineData, Health>>
    {
    public:
        Propulsion(Starship& ship, accel_vec mDesiredAcceleration)
                : mShip(ship), mDesiredAcceleration(mDesiredAcceleration)
        {}

        void apply(const ComponentEntity& ce, const EngineData& engine, const Health& health)
        {
            auto dt = 0.1_s;
            force_t want    = length(mDesiredAcceleration) * mShip.mass();
            // if we do not want to accelerate, end here
            if(want == force_t(0)) {
                return;
            }

            mass_t need_mass  = (want / engine.mPropellantSpeed) * dt;
            mass_t max_mass   = engine.mMassRate * dt * health.status();
            if(need_mass > max_mass)
                need_mass = max_mass;
            mass_t fuel = mShip.getTank().requestFuel( need_mass );

            force_t f = fuel * engine.mPropellantSpeed  / dt;
            mMaxAcceleration += max_mass * engine.mPropellantSpeed  / dt / mShip.mass();
            mProducedAcceleration += mDesiredAcceleration * (f / want);
        }

        const accel_vec& getProduced() const { return mProducedAcceleration; }
        accel_t getMax() const { return mMaxAcceleration; }
    private:
        Starship& mShip;
        accel_vec mDesiredAcceleration;
        accel_vec mProducedAcceleration{.0, .0, .0};
        accel_t   mMaxAcceleration{.0};
    };

}

// ---------------------------------------------------------------------------------------------------

SubSystems::SubSystems(const boost::property_tree::ptree& data):
        mEngine(new Engine(data.get_child("engine"))),
        mShield(new ShieldGenerator(data.get_child("shieldgen"))),
        mPowerPlant(new PowerPlant(data.get_child("power_plant"))),
        mFuelTank(new FuelTank(data.get_child("tank"))),
        mLifeSupport( new LifeSupport(data.get_child("life_support")) )
{
    mArmament.push_back(std::make_unique<ProjectileWeapon>(data.get_child("weapon")));

    mCompPtrs.push_back( mEngine.get() );
    mCompPtrs.push_back( mShield.get() );
    mCompPtrs.push_back( mPowerPlant.get() );
    mCompPtrs.push_back( mLifeSupport.get() );
    mCompPtrs.push_back( mFuelTank.get() );
    for(auto& wpn : mArmament)
        mCompPtrs.push_back( wpn.get() );
}

namespace {
    template<class T>
    std::unique_ptr<T> clone(const std::unique_ptr<T>& original)
    {
        return std::unique_ptr<T>(original->clone());
    }

    template<class T>
    std::vector<T> clone(const std::vector<T>& original)
    {
        std::vector<T> cl;
        cl.reserve(original.size());
        std::transform(begin(original), end(original),
                       std::back_inserter(cl), [](const T& t) { return clone(t); });
        return std::move(cl);
    }
}

SubSystems::SubSystems( const SubSystems& other ):
        mEngine( clone(other.mEngine) ),
        mShield( clone(other.mShield) ),
        mPowerPlant( clone(other.mPowerPlant) ),
        mFuelTank( clone(other.mFuelTank) ),
        mLifeSupport( clone(other.mLifeSupport) ),
        mArmament( clone(other.mArmament) )
{
    mCompPtrs.push_back( mEngine.get() );
    mCompPtrs.push_back( mShield.get() );
    mCompPtrs.push_back( mPowerPlant.get() );
    mCompPtrs.push_back( mLifeSupport.get() );
    mCompPtrs.push_back( mFuelTank.get() );
    for(auto& wpn : mArmament)
        mCompPtrs.push_back( wpn.get() );
}

double SubSystems::distributeEnergy(double energy)
{
    CollectRequests crq;

    double energy_to_distribute = energy;
    for(auto& sys : mCompPtrs) {
        crq(sys->entity());
    }

    auto requests = crq.requests();
    // energy distribution according to demand and priority
    std::vector<double> supply(requests.size(), 0.f );

    const int num_iters = 10;
    for(unsigned p = 0; p < num_iters; ++p)
    {
        double total_request = accumulate( begin(requests), end(requests), 0.0 );
        if(total_request == 0)
            break;

        double edist = (p+1) / num_iters * energy_to_distribute;
        for(unsigned i = 0; i < supply.size(); ++i)
        {
            if(requests[i] == 0) continue;

            double add = requests[i] * edist / total_request * crq.priority()[i];

            if(supply[i] + add > crq.requests()[i]) {
               add = crq.requests()[i] - supply[i];
            }
            if(supply[i] + add > energy_to_distribute)
                add = energy_to_distribute - supply[i];

            supply[i] += add;
            requests[i] -= add;
            energy_to_distribute -= add;
        }
    }

    ProvideEnergy pv(supply);
    // supply energy to systems
    for(auto& sys : mCompPtrs) {
        pv(sys->entity());
    }

    return accumulate(begin(supply), end(supply), 0.0);
}

double SubSystems::produceEnergy() const
{
    GetProvidedEnergy gpe;
    // get excess energy from each component
    for(auto& sys : mCompPtrs) {
        gpe(sys->entity());
    }

    return gpe.energy();
}

void SubSystems::onStep(Starship& ship)
{
    TimerCountdown tc(0.1);
    ShieldManagement smgm(ship);
    LifeSupportStep ls(ship);
    PowerProduction pp;
    Propulsion prop(ship, ship.getDesiredAcceleration());
    for(auto& c : mCompPtrs)
    {
        tc(c->entity());
        smgm(c->entity());
        ls(c->entity());
        pp(c->entity());
        prop(c->entity());
    }

    ship.setProducedAcceleration( prop.getProduced() );
    ship.setMaxAcceleration( prop.getMax() );
}
