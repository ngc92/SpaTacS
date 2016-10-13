//
// Created by erik on 9/26/16.
//

#ifndef SOI_PHYSICSOBJECT_H
#define SOI_PHYSICSOBJECT_H

#include "units.h"
#include "Fixture.h"
#include <vector>

namespace spatacs {
    namespace physics {
        class Object
        {
        public:
            Object() = default;
            Object(const length_vec& p, const velocity_vec& v, mass_t m, std::uint64_t udata = 0);

            // userdata
            std::uint64_t userdata() const;
            void setUserdata(std::uint64_t value);

            // ID
            std::uint64_t id() const;
            /// \note this should be called only once, when the object is added to the world
            void setID(std::uint64_t id);

            // current step base data
            const length_vec& position() const;
            const velocity_vec& velocity() const;

            // interpolated data
            length_vec position(time_t dt) const;

            // these should be used with care.
            void setPosition(const length_vec& p);
            void setVelocity(const velocity_vec& v);

            // mass
            mass_t mass() const;
            void setMass(mass_t mass);

            // iterate over fixtures
            auto begin() const { return mFixtures.begin(); }
            auto end()   const { return mFixtures.end();   }

            // adding a fixture
            Fixture& addFixture(length_t radius);

        private:
            std::uint64_t mID = 0;
            length_vec mPosition{0.0_m, 0.0_m, 0.0_m};
            velocity_vec mVelocity{0.0_kps, 0.0_kps, 0.0_kps};
            mass_t mMass = 1.0_kg;

            // fixtures
            std::vector<Fixture> mFixtures;

            std::uint64_t mUserData;
        };
    }
}

#endif //SOI_PHYSICSOBJECT_H
