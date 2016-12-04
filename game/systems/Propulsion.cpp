//
// Created by erik on 12/3/16.
//

#include "Propulsion.h"
#include "TankInfo.h"
#include "game/Starship.h"
#include "game/SubSystems.h"

using namespace spatacs;
using namespace game::systems;

Propulsion::Propulsion(Starship& ship, accel_vec mDesiredAcceleration)
        : mShip(ship), mDesiredAcceleration(mDesiredAcceleration)
{}

void Propulsion::operator()(EngineData& engine, const Health& health, const Activity& acc)
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

    // get fuel
    FuelRequest request{need_mass};
    mShip.components().apply(request);
    mass_t fuel = request.fuel();

    force_t f = fuel * engine.propellant_speed  / dt;
    mMaxAcceleration += max_mass * engine.propellant_speed  / dt / mShip.mass();
    mProducedAcceleration += mDesiredAcceleration * (f / want);
}

