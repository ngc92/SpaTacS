//
// Created by erik on 10/1/16.
//

#ifndef SOI_DAMAGE_H
#define SOI_DAMAGE_H

namespace spatacs
{
    namespace game
    {
        /* Projectile Damage works as follows:
         *    The most simple form of damage is shield overload:
         *      It reduces the enemy shield by the given amount, and
         *      has no other effect.
         *    Next is HE damage. This does 50% damage against shields,
         *      full structural damage but only 20% armour.
         *    The kinetic energy of the projectile is translated to kinetic damage.
         *      Kinetic damage affects shield with 50%, armour with 33% and does
         *      full structural damage.
         *    Further, there exists the AP factor. This determines the likelihood
         *      that the shot penetrates the armour and does structural damage directly.
         *
        */
        struct Damage
        {
            double high_explosive  = 0;
            double shield_overload = 0;
            double kinetic         = 0;
            double armour_pierce   = 0;
        };
    }
}

#endif //SOI_DAMAGE_H
