//
// Created by erik on 12/3/16.
//

#include <game/Shield.h>
#include "Shield.h"
#include "game/Starship.h"
#include "game/SubSystems.h"

using namespace spatacs;
using namespace game::systems;

void ShieldManagement::operator()(ShieldGeneratorData& sgen, const Health& health, const Activity& acc,
                                  Shield& shield, EnergyManager& emgr)
{
    auto dt = 0.1_s;
    if(shield.current() < shield.maximum())
    {
        double difference = shield.maximum() - shield.current();
        double recharge = sgen.mShieldRecharge * dt * health.status() * acc.get();
        double rec = std::min( recharge, difference );
        energy_t consumption = rec * sgen.mEnergyPerShieldPoint;
        if(!(consumption == 0.0_J)) {
            rec *= emgr.requestPower(consumption) / consumption;
        }
        shield.setShield( shield.current() + rec );
    }
}
