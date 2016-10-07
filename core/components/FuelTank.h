//
// Created by erik on 10/5/16.
//

#ifndef SPATACS_FUELTANK_H
#define SPATACS_FUELTANK_H

#include "IComponent.h"
#include "physics/units.h"

namespace spatacs
{
namespace core
{
    class FuelTank : public IComponent
    {
    public:
        FuelTank(const ptree& props);
        void onStep(Starship& ship) override { };
        FuelTank* clone() const override;

        mass_t requestFuel(mass_t req);
        mass_t fuel() const;
        mass_t capacity() const;
    private:
        mass_t mFuelCapacity;
        mass_t mCurrentFuel;
    };
}
}

#endif //SPATACS_FUELTANK_H
