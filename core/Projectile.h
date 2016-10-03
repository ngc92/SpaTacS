//
// Created by erik on 9/20/16.
//

#ifndef SOI_PROJECTILE_H
#define SOI_PROJECTILE_H

#include "vec.h"
#include "physics/Object.h"
#include "physics/units.h"
#include "GameObject.h"
#include "Damage.h"

namespace spatacs
{
namespace core
{
    class Projectile : public GameObject
    {
    public:
        Projectile() = default;
        Projectile(std::uint64_t id, std::uint64_t shooter, Damage damage);
        void onStep();

        float age() const;
        const Damage& damage() const;

        std::uint64_t shooter() const;

        void expire();

    private:
        float mAge = 0;

        // save shooter to prevent self-hits in the first frame
        std::uint64_t mShooterId;

        // damage info
        Damage mDamage;
    };
}
}

#endif //SOI_PROJECTILE_H
