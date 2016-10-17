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

        force_t max_thrust() const;
        void setTargetAccel(accel_vec a);

        virtual Engine* clone() const override;

    private:
        // engine config
        speed_t mPropellantSpeed = 100.0_kps;
        rate_t<mass_t> mMassRate = 1.0_kg / 1.0_s;

        // engine status
        accel_vec mDesiredAcceleration;
    };
}
}

#endif //SOI_ENGINE_H
