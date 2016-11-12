//
// Created by erik on 10/18/16.
//

#include "systems.h"
#include "Starship.h"
#include "SubSystems.h"

using namespace spatacs;
using namespace game;

// General purpose components
TimerCountdown::TimerCountdown(double dt) : mDeltaT(dt) { }

void TimerCountdown::apply(Timer& timer) const
{
    timer.time -= mDeltaT;
}

// ------------------------------------------------------------------------

Propulsion::Propulsion(Starship& ship, accel_vec mDesiredAcceleration)
        : mShip(ship), mDesiredAcceleration(mDesiredAcceleration)
{}

mass_t spatacs::game::getFuel(core::EntityManager<ComponentEntity>& entities, mass_t request)
{
    mass_t fuel = 0.0_kg;

    auto sys = core::make_system<game::ComponentEntity, game::FuelStorage>
            ( [&](game::FuelStorage& s) {
                auto get = s.request(request);
                request -= get;
                fuel += get;
            });
    entities.apply(sys);


    return fuel;
}

void Propulsion::apply(EngineData& engine, const Health& health, const Activity& acc)
{
    auto dt = 0.1_s;
    force_t want = length(mDesiredAcceleration) * mShip.mass();
    // if we do not want to accelerate, end here
    if(want == force_t(0)) {
        return;
    }

    mass_t need_mass  = (want / engine.propellant_speed) * dt;
    mass_t max_mass   = engine.mass_rate * dt * health.status() * acc.get();
    if(need_mass > max_mass)
        need_mass = max_mass;

    mass_t fuel = getFuel(mShip.components(), need_mass);
    force_t f = fuel * engine.propellant_speed  / dt;
    mMaxAcceleration += max_mass * engine.propellant_speed  / dt / mShip.mass();
    mProducedAcceleration += mDesiredAcceleration * (f / want);
}


ShieldManagement::ShieldManagement(Starship& s, EnergyManager& e) : ship(s), emgr(e) { }
void ShieldManagement::apply(ShieldGeneratorData& sgen, const Health& health, const Activity& acc)
{
    auto dt = 0.1_s;
    // shield decay
    auto shield = ship.shield();
    if(shield < ship.max_shield())
    {
        double difference = ship.max_shield() - ship.shield();
        double recharge = sgen.mShieldRecharge * dt * health.status() * acc.get();
        double rec = std::min( recharge, difference );
        energy_t consumption = rec * sgen.mEnergyPerShieldPoint;
        if(!(consumption == 0.0_J)) {
            rec *= emgr.requestPower(consumption) / consumption;
        }
        shield += rec;
    }
    ship.setShield( shield );
}

PowerProduction::PowerProduction(core::EntityManager<ComponentEntity>& entities) : mEntites(entities)
{

}

void PowerProduction::apply(const PowerPlantData& pp, const Health& health, const Activity& acc)
{
    auto base_energy = 0.1_s * pp.energy_production * acc.get();
    mass_t need_fuel = base_energy * 10.0_kg / 1.0_MJ;
    mass_t fuel = getFuel(mEntites, need_fuel);
    if(need_fuel < 0.001_kg) need_fuel = 0.001_kg;
    mProducedEnergy += base_energy * health.status() * fuel / need_fuel;
}

LifeSupportStep::LifeSupportStep(const Starship& s, EnergyManager& e) : ship(s), emgr(e) { }

void LifeSupportStep::apply(LifeSupportData& sup) const
{
    auto accel = length(ship.velocity() - sup.mLastVelocity) / 0.1_s + 9.81_m/(1.0_s)/(1.0_s);
    sup.mLastVelocity = ship.velocity();
    energy_t ereq = accel * 100.0_kg * 100.0_m;
    /// \todo just faking the units here
    /// \todo for now, nothing happens when we do not get the requested energy!
    emgr.requestPower(ereq);
}

void TankInfo::apply(const FuelStorage& h)
{
    mFuel     += h.current;
    mCapacity += h.capacity;
}

Aiming::Aiming(length_vec position, velocity_vec velocity) :
        mTargetPos(position),
        mTargetVel(velocity)
{
}

void Aiming::apply(const game::WeaponAimData& aim, const game::Health& health)
{
    // perfect aim
    speed_t vel = aim.muzzle_velocity; // km/s
    auto d = length(mTargetPos);
    auto aim_pos = mTargetPos;

    for(unsigned i = 0; i < 3; ++i) {
        aim_pos = mTargetPos + (d / vel) * mTargetVel;
        auto old_d = d;
        d = length(aim_pos);
        //std::cout << "correct: " << i << " " << abs(d - old_d) << "\n";
        if( abs(d - old_d) < 10.0_m )
            break;
    }



    // miss-aim
    auto precision = aim.precision * (health.status() + 1) * .5;
    length_t aim_radius = d / precision;
    auto aim_radius_miss = (rand() % 1001 / 1000.0) * aim_radius;

    auto miss_base = meters(rand() % 1001 - 500.0, rand() % 1001 - 500.0, rand() % 1001 - 500.0);
    auto miss = perpendicular(miss_base, aim_pos);

    length_t n = length(miss);
    if( n == 0.0_m ) miss *= 0.0;
    else miss *= aim_radius_miss / n;

    mAimPos = aim_pos + miss;
    mMuzzleVel = vel;
}

void ListAmmunition::apply(const AmmoStorage& as)
{
    mCapacity += as.capacity;
    for(auto& ammo : as.ammo)
    {
        mAmmos.push_back(ammo);
    }
}

void AddAmmunition::apply(AmmoStorage& as)
{
    as.addAmmo(mData, mAmount);
}