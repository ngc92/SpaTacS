//
// Created by erik on 1/3/17.
//

#include "Propulsion.h"
#include "../subcomponents.h"
#include "../managers.h"

using namespace spatacs;
using namespace game::systems;

void EngineThrust::operator()(EngineData& engine, const Health& health, const Activity& acc, FuelDistributor& fuelmgr,
                            const force_vec& desired_force)
{
    auto dt = 0.1_s;
    force_t want = length(desired_force);
    // if we do not want to accelerate, end here
    if(want == force_t(0)) {
        return;
    }

    mass_t need_mass  = (want / engine.propellant_speed) * dt;
    mass_t max_mass   = engine.mass_rate * dt * health.status() * acc.get();
    if(need_mass > max_mass)
        need_mass = max_mass;

    // get fuel
    mass_t fuel = fuelmgr.request(need_mass);

    force_t f = fuel * engine.propellant_speed  / dt;
    mMaxForce += max_mass * engine.propellant_speed  / dt;
    mProducedForce += desired_force * (f / want);
}