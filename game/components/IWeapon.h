//
// Created by erik on 9/20/16.
//

#ifndef SOI_IWEAPON_H_H
#define SOI_IWEAPON_H_H

#include "game/components/IComponent.h"
#include <memory>
#include "vec.h"
#include "game/Damage.h"
#include <boost/optional.hpp>
#include "physics/units.h"

namespace spatacs
{
    namespace game
    {
        struct ShotData
        {
            velocity_vec vel;
            Damage damage;
        };

        class IWeapon : public IComponent
        {
        public:
            using IComponent::IComponent;
            virtual  ~IWeapon() = default;

            virtual IWeapon* clone() const override = 0;
            virtual void setMode( std::uint64_t mode ) = 0;

            // firing status
            virtual bool ready() const = 0;
            virtual boost::optional<ShotData> fire(const length_vec& delta_p, const velocity_vec& delta_v) = 0;

            // info retrieval
            virtual float hit_chance(length_t distance, area_t xsec) const = 0;
        };
    }
}

#endif //SOI_IWEAPON_H_H
