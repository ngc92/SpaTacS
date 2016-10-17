//
// Created by erik on 9/19/16.
//

#ifndef SOI_WEAPON_H
#define SOI_WEAPON_H

#include <physics/units.h>
#include "game/components/IWeapon.h"
#include "game/Damage.h"

namespace spatacs
{
    namespace game
    {
        class ProjectileWeapon : public IWeapon
        {
        public:
            void onStep(Starship& ship);

            ProjectileWeapon( const ptree& props );
            ~ProjectileWeapon() = default;

            ProjectileWeapon* clone() const override;

            double precision() const;

            bool  ready() const override;

            float hit_chance(length_t distance, area_t xsec) const override;

            virtual void setMode(std::uint64_t mode) override;

            boost::optional<ShotData> fire(const length_vec& delta_p,
                                             const velocity_vec& delta_v) override;
        };
    }
}

#endif //SOI_WEAPON_H
