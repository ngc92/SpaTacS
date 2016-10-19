//
// Created by erik on 10/18/16.
//

#include "systems.h"
#include "Starship.h"

using namespace spatacs;
using namespace game;

// General purpose components
TimerCountdown::TimerCountdown(double dt) : mDeltaT(dt) { }

void TimerCountdown::apply(const ComponentEntity& ety, Timer& timer) const
{
    timer.time -= mDeltaT;
}

// ------------------------------------------------------------------------

Propulsion::Propulsion(const Starship& ship, accel_vec mDesiredAcceleration)
        : mShip(ship), mDesiredAcceleration(mDesiredAcceleration)
{}

void Propulsion::apply(const ComponentEntity& ce, const EngineData& engine,
                       const Health& health, FuelRequest& freq)
{
    auto dt = 0.1_s;
    force_t want = length(mDesiredAcceleration) * mShip.mass();
    // if we do not want to accelerate, end here
    if(want == force_t(0)) {
        return;
    }

    mass_t need_mass  = (want / engine.mPropellantSpeed) * dt;
    mass_t max_mass   = engine.mMassRate * dt * health.status();
    if(need_mass > max_mass)
        need_mass = max_mass;

    freq.request = need_mass;
    mass_t fuel = freq.get(need_mass);

    force_t f = fuel * engine.mPropellantSpeed  / dt;
    mMaxAcceleration += max_mass * engine.mPropellantSpeed  / dt / mShip.mass();
    mProducedAcceleration += mDesiredAcceleration * (f / want);
}


ShieldManagement::ShieldManagement(Starship& s) : ship(s) { }
void ShieldManagement::apply(const ComponentEntity& ety, ShieldGeneratorData& sgen,
           EnergyManagement& egy, const Health& health)
{
    auto dt = 0.1_s;
    // shield decay
    auto shield = ship.shield();
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

void PowerProduction::apply(const ComponentEntity& ety, PowerPlantData& pp,
                            EnergyManagement& egy, const Health& health) const
{
    egy.cache += 0.1f * pp.energy_production * health.status();;
}

FuelDistribution::FuelDistribution(mass_t available) : mFuel(available)
{
}

void FuelDistribution::apply(const game::ComponentEntity& ety, game::FuelRequest& h)
{
    auto desire = h.request - h.current;
    desire = std::min(desire, mFuel);
    mFuel -= desire;
    h.current += desire;
}

FuelConsumption::FuelConsumption(mass_t consume) : mConsume(consume)
{}

void FuelConsumption::apply(const game::ComponentEntity& ety, game::FuelStorage& h)
{
    mConsume -= h.request(mConsume);
}

LifeSupportStep::LifeSupportStep(const Starship& s) : ship(s) { }

void LifeSupportStep::apply(const ComponentEntity& ety, LifeSupportData& sup, EnergyManagement& egy) const
{
    auto accel = length(ship.velocity() - sup.mLastVelocity) / 0.1_s + 9.81_m/(1.0_s)/(1.0_s);
    sup.mLastVelocity = ship.velocity();
    double ereq = accel.value / 1000.0;
    /// \todo for now, nothing happens when we do not get the requested energy!
    egy.requestEnergy( ereq );
}

void TankInfo::apply(const game::ComponentEntity& ety, const game::FuelStorage& h)
{
    mFuel     += h.current;
    mCapacity += h.capacity;
}