//
// Created by erik on 9/18/16.
//

#ifndef SOI_ENGINE_H
#define SOI_ENGINE_H

#include "IComponent.h"
#include "physics/units.h"

namespace spatacs
{
namespace core
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
        force_t mMaxThrust{1.0};
        float mEnergyConsumption = 2;

        // engine status
        force_t mUnusedThrust{1.0};
    };
}
}

#endif //SOI_ENGINE_H
