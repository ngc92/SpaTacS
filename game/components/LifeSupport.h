//
// Created by erik on 10/6/16.
//

#ifndef SPATACS_LIFESUPPORT_H
#define SPATACS_LIFESUPPORT_H

#include "game/components/IComponent.h"
#include "physics/units.h"

namespace spatacs
{
namespace game
{
    class LifeSupport : public IComponent
    {
    public:
        LifeSupport( const ptree& dat );
        void onStep(Starship& ship) override;

        LifeSupport* clone() const override;

    private:
        velocity_vec mLastVelocity;
        /// \todo add oxygen and temperature
    };
}
}


#endif //SPATACS_LIFESUPPORT_H
