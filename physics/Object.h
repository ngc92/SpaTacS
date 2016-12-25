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
            /// \throw std::logic_error, if \p id is an invalid ID
            /// \thorw std::logic_error, if this Object has already an assigned ID.
            void setID(ObjectID id);

            // current step base data
            /// Get the current (base) position of the Object.
            const length_vec& position() const;
            /// Get the current (base) velocity of the Object.
            const velocity_vec& velocity() const;

            // these should be used with care.
            /// Set the current (base) position of the Object.
            void setPosition(const length_vec& p);
            /// Set the current (base) velocity of the Object.
            void setVelocity(const velocity_vec& v);

            // mass
            /// Get the mass of the Object.
            mass_t mass() const;
            /// Set the mass of the Object.
            void setMass(mass_t mass);

            // iterate over fixtures
            auto begin() const { return mFixtures.begin(); }
            auto end()   const { return mFixtures.end();   }

            // adding a fixture
            Fixture& addFixture(length_t radius);

        private:
            ObjectID mID{};
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
