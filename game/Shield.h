//
// Created by erik on 12/10/16.
//

#ifndef SPATACS_GAME_SHIELD_H
#define SPATACS_GAME_SHIELD_H

#include "GameObject.h"

namespace spatacs
{
namespace game
{
    class Shield : public GameObject
    {
    public:
        Shield(ObjectID ship);
        bool alive() const override;
        void onStep(GameState& state) override;
        Shield *clone() const override;

        double current() const;
        double maximum() const;
        void setShield( double new_value );

        length_t radius() const;
        void setRadius(length_t radius);
    private:
        ObjectID mShipID;

        // shield data
        double mMaxShield = 10;
        double mCurShield = 10;
        rate_t<scalar_t> mShieldDecay{0.0};
        length_t mRadius;

    };
}
}

#endif //SPATACS_GAME_SHIELD_H
