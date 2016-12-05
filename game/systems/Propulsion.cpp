//
// Created by erik on 12/3/16.
//

#include "Propulsion.h"
#include "FuelTank.h"
#include "game/Starship.h"

using namespace spatacs;
using namespace game::systems;

void Propulsion::operator()(EngineData& engine, const Health& health, const Activity& acc,
                            Starship& ship, const accel_vec& desired_acceleration)
{
    auto dt = 0.1_s;
    force_t want = length(desired_acceleration) * ship.mass();
    // if we do not want to accelerate, end here
    if(want == force_t(0)) {
        return;
    }

    mass_t need_mass  = (want / engine.propellant_speed) * dt;
    mass_t max_mass   = engine.mass_rate * dt * health.status() * acc.get();
    if(need_mass > max_mass)
        need_mass = max_mass;

    // get fuel
    mass_t fuel = request_fuel(ship.components(), need_mass);

    force_t f = fuel * engine.propellant_speed  / dt;
    mMaxAcceleration += max_mass * engine.propellant_speed  / dt / ship.mass();
    mProducedAcceleration += desired_acceleration * (f / want);
}

