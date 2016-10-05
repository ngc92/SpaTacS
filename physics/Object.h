//
// Created by erik on 9/26/16.
//

#ifndef SOI_PHYSICSOBJECT_H
#define SOI_PHYSICSOBJECT_H

#include "vec.h"
#include "units.h"

namespace spatacs {
    namespace physics {
        class Object
        {
        public:
            Object() = default;
            Object(const length_vec& p, const velocity_vec& v, length_t r, mass_t m, std::uint64_t udata = 0);

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

            // simulate for a time
            void simulate(time_t dt);

            // shape data
            length_t radius() const;
            void setRadius(length_t rad);
        private:
            std::uint64_t mID = 0;
            length_vec mPosition{0.0_m, 0.0_m, 0.0_m};
            velocity_vec mVelocity{0.0_kps, 0.0_kps, 0.0_kps};
            length_t mRadius{0.01f};
            mass_t mMass = 1.0_kg;
            std::uint64_t mUserData;
        };
    }
}

#endif //SOI_PHYSICSOBJECT_H
