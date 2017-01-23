//
// Created by erik on 12/31/16.
//

#include "game/entity/components.h"
#include "physics/PhysicsWorld.h"
#include "Propulsion.h"

#include "core/ecs/EntityManager.h"

using namespace spatacs;
using namespace game;

namespace
{
    struct Thrust
    {
        force_vec produced;
        force_t   maximum;
    };

    using namespace components;
    Thrust get_thrust(const Engine& engine, FuelTank& tank, const force_vec& desired_force)
    {
        auto dt = 0.1_s;
        force_t want = length(desired_force);
        // if we do not want to accelerate, end here
        if(want == force_t(0)) {
            return Thrust{desired_force, force_t{0}};
        }

        mass_t need_mass  = (want / engine.propellant_speed) * dt;
        mass_t max_mass   = engine.mass_rate * dt;
        if(need_mass > max_mass)
            need_mass = max_mass;

        // get fuel
        mass_t fuel = tank.request(need_mass);

        force_t f = fuel * engine.propellant_speed  / dt;
        auto maximum = max_mass * engine.propellant_speed  / dt;
        auto produced = desired_force * (f / want);

        return Thrust{produced, maximum};
    }
}

void systems::ApplyPropulsion::operator()(PhysicsData& pd, PropulsionControl& pc, const Engine& engine, FuelTank& tank) const
{
    auto desired_force = pc.desired_acceleration() * pd.mass();

    auto thrust = get_thrust(engine, tank, desired_force);

    pc.setMaximumAcceleration( thrust.maximum / pd.mass() );
    pd.setForce( pd.force() + thrust.produced );
}
