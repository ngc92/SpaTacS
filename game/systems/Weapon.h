//
// Created by erik on 12/3/16.
//

#ifndef SPATACS_GAME_WEAPON_H
#define SPATACS_GAME_WEAPON_H

#include "fwd.h"

namespace spatacs
{
namespace game
{
namespace systems
{
    namespace signatures
    {
        using AimSignature    = core::ecs::Signature<const WeaponAimData, const Health>;
    }

    class Aiming : public core::ecs::System<signatures::AimSignature>
    {
    public:
        Aiming(length_vec position, velocity_vec velocity);
        void operator()(const game::WeaponAimData& aim, const game::Health& health);

        const length_vec& aim_pos() const { return mAimPos; }
        const speed_t     speed() const { return mMuzzleVel; }
    private:
        length_vec mTargetPos;
        velocity_vec mTargetVel;

        length_vec mAimPos{.0, .0, .0};
        speed_t mMuzzleVel{.0};
    };
}
}
}

#endif //SPATACS_GAME_WEAPON_H
