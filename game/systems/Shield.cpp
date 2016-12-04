//
// Created by erik on 12/3/16.
//

#include "Shield.h"
#include "game/Starship.h"
#include "game/SubSystems.h"

using namespace spatacs;
using namespace game::systems;


ShieldManagement::ShieldManagement(Starship& s, EnergyManager& e) :
        ship(s), emgr(e)
{
}

void ShieldManagement::operator()(ShieldGeneratorData& sgen, const Health& health, const Activity& acc)
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
