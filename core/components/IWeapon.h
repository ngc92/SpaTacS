//
// Created by erik on 9/20/16.
//

#ifndef SOI_IWEAPON_H_H
#define SOI_IWEAPON_H_H

#include "IComponent.h"
#include <memory>
#include "vec.h"
#include "core/Damage.h"
#include <boost/optional.hpp>

namespace spatacs
{
    namespace core
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

            virtual void onStep(Starship& ship) = 0;

            virtual IWeapon* clone() const override = 0;
            virtual void setMode( std::uint64_t mode ) = 0;
            virtual std::uint64_t mode() const = 0;

            // firing status
            virtual bool ready() const = 0;
            virtual boost::optional<ShotData> fire(const length_vec& delta_p, const velocity_vec& delta_v) = 0;

            // info retrieval
            virtual float strength(length_t distance, area_t xsec) const = 0;
            virtual float hit_chance(length_t distance, area_t xsec) const = 0;
        };
    }
}

#endif //SOI_IWEAPON_H_H
