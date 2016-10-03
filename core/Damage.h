//
// Created by erik on 10/1/16.
//

#ifndef SOI_DAMAGE_H
#define SOI_DAMAGE_H

namespace spatacs
{
    namespace core
    {
        struct Damage
        {
            float high_explosive  = 0;
            float shield_overload = 0;
            float armour_piercing = 0;
            float kinetic         = 0;
        };
    }
}

#endif //SOI_DAMAGE_H
