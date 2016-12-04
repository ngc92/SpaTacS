//
// Created by erik on 12/3/16.
//

#include "LifeSupport.h"
#include "game/Starship.h"
#include "game/SubSystems.h"

using namespace spatacs;
using namespace game::systems;

LifeSupportStep::LifeSupportStep(const Starship& s, EnergyManager& e) :
        ship(s), emgr(e)
{
}

void LifeSupportStep::operator()(LifeSupportData& sup) const
{
    auto accel = length(ship.velocity() - sup.mLastVelocity) / 0.1_s + 9.81_m/(1.0_s)/(1.0_s);
    sup.mLastVelocity = ship.velocity();
    energy_t ereq = accel * 100.0_kg * 100.0_m;
    /// \todo just faking the units here
    /// \todo for now, nothing happens when we do not get the requested energy!
    emgr.requestPower(ereq);
}
