//
// Created by erik on 10/5/16.
//

#ifndef SPATACS_FUELTANK_H
#define SPATACS_FUELTANK_H

#include "IComponent.h"

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

        double getFuel( double req );

    private:
        double mFuelCapacity;
        double mCurrentFuel;
    };
}
}

#endif //SPATACS_FUELTANK_H
