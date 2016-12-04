//
// Created by erik on 12/3/16.
//

#include "Weapon.h"
#include "../components.h"

using namespace spatacs;
using namespace game::systems;

Aiming::Aiming(length_vec position, velocity_vec velocity) :
        mTargetPos(position),
        mTargetVel(velocity)
{
}

void Aiming::operator()(const game::WeaponAimData& aim, const game::Health& health)
{
    // perfect aim
    speed_t vel = aim.muzzle_velocity;
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