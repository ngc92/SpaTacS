//
// Created by erik on 10/29/16.
//

#include <algorithm>
#include <cassert>
#include "Damage.h"

using namespace spatacs;

void  game::applyDamage(double& damage, double factor, double& target)
{
    double dmg = std::min(target, damage*factor);
    damage -= dmg / factor;
    target -= dmg;
}

game::DamageEffect game::getShieldDamage(const Damage& d, double shield)
{
    DamageEffect de;
    de.remaining = d;

    // next, let shield stop the HE part. HE only does 50% damage against shield
    double& so = de.remaining.shield_overload;
    double& he = de.remaining.high_explosive;
    double& kd = de.remaining.kinetic;
    double os = shield;

    // first, apply shield overload. This affects the whole shield
    applyDamage(so, 1.0, os);

    // only a portion of the shield can block at any given time
    /// \todo make this radius dependent?
    double block = os * 0.2;
    os -= block;
    applyDamage(kd, 0.5, block);
    os += block;
    if(kd == 0) {
        applyDamage(he, 0.5, os);
    }

    de.applied = shield - os;
    return de;
}

double game::getPierceProbability(double AP, double armour, length_t radius)
{
    auto area = 4 * 3.1415 * radius * radius;
    auto armour_density = scalar_t(armour) / area;
    double rel_strength = scalar_t(AP + 0.2) / (armour_density * 65.0_m * 65.0_m);
    if(rel_strength > 1)
        rel_strength = 1.0;

    return rel_strength;
}

game::DamageEffect game::getArmourDamage(const Damage& d, double armour, length_t target_radius)
{
    DamageEffect de;
    de.remaining = d;
    double& he = de.remaining.high_explosive;
    double& kd = de.remaining.kinetic;
    assert(d.armour_pierce <= 1.0);
    double ap = de.remaining.kinetic * de.remaining.armour_pierce;

    double initial_armour = armour;

    int pierce_prob = int(100 * getPierceProbability(ap, armour, target_radius));

    if(rand() % 100 < pierce_prob)
    {
        // in the piercing case, only 50% of the AP
        // damage damage the armour, and the remaining KD
        // goes to structure
        ap *= 0.5;
        double old_ap = ap;
        applyDamage(ap, 1.0, armour);
        kd -= old_ap - ap;
    } else {
        double old_ap = ap;
        applyDamage(ap, 1.0, armour);
        kd -= old_ap - ap;
        applyDamage(kd, 0.33, armour);
    }

    applyDamage(he, 0.20, armour);

    de.applied = initial_armour - armour;
    return de;
}