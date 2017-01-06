//
// Created by erik on 1/6/17.
//

#ifndef SPATACS_GAME_ENTITY_SUBS_MANAGERS_H
#define SPATACS_GAME_ENTITY_SUBS_MANAGERS_H

#include "physics/units.h"

namespace spatacs
{
    namespace game
    {
        class FuelDistributor
        {
        public:
            mass_t request(mass_t amount);

            mass_t requested() const;
            mass_t reserve() const;
            void provide(mass_t);
        private:
            mass_t mCache = 0.0_kg;
            mass_t mTotalRequests = 0.0_kg;
        };

        class PowerDistributor
        {
        public:

            // provides the energy system with new energy. Resets the cache.
            energy_t supply(energy_t);

            energy_t request(energy_t amount);

            energy_t produced() const;
            energy_t requested() const;

        private:
            energy_t mTotalProduced{0};
            energy_t mTotalRequested{0};
            energy_t mCache{0};
        };
    }
}

#endif //SPATACS_GAME_ENTITY_SUBS_MANAGERS_H
