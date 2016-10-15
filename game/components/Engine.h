//
// Created by erik on 9/18/16.
//

#ifndef SOI_ENGINE_H
#define SOI_ENGINE_H

#include "IComponent.h"
#include "physics/units.h"

namespace spatacs
{
namespace game
{
    class Engine : public IComponent
    {
    public:
        Engine() = default;
        Engine(const ptree& props);

        void onStep(Starship& ship) override;

        force_vec getThrust(const force_vec& accel);
        force_t max_thrust() const;

        virtual Engine* clone() const override;

    private:
        // engine config
        speed_t mPropellantSpeed = 100.0_kps;
        rate_t<mass_t> mMassRate = 1.0_kg / 1.0_s;

        // engine status
        mass_t mUnusedMass       = 0.0_kg;
    };
}
}

#endif //SOI_ENGINE_H
