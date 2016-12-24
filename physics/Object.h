//
// Created by erik on 9/26/16.
//

#ifndef SPATACS_PHYSICS_OBJECT_H
#define SPATACS_PHYSICS_OBJECT_H

#include "units.h"
#include "Fixture.h"
#include <vector>
#include "ObjectID.h"

namespace spatacs
{
    namespace physics
    {
        /*! \brief Class that represents a physics object.
         *  \details This is mostly a dumb container that collects the data for a physics object.
         *          Most functions are just getters or setters that may do some simple sanity checks.
         */
        class Object
        {
        public:
            Object() = default;
            Object(const length_vec& p, const velocity_vec& v, mass_t m, std::uint64_t udata = 0);

            // userdata
            std::uint64_t userdata() const;
            void setUserdata(std::uint64_t value);

            // ID
            ObjectID id() const;
            /// \note this should be called only once, when the object is added to the world.
            void setID(ObjectID id);

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
            ObjectID mID{0};
            length_vec mPosition{0.0_m, 0.0_m, 0.0_m};
            velocity_vec mVelocity{0.0_kps, 0.0_kps, 0.0_kps};
            mass_t mMass = 1.0_kg;

            // fixtures
            std::vector<Fixture> mFixtures;

            std::uint64_t mUserData;
        };
    }
}

#endif //SPATACS_PHYSICS_OBJECT_H
