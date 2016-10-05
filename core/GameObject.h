//
// Created by erik on 9/28/16.
//

#ifndef SOI_GAMEOBJECT_H
#define SOI_GAMEOBJECT_H

#include <cstdint>
#include "physics/Object.h"

namespace spatacs
{
    namespace core
    {
        class GameObject
        {
        public:
            virtual ~GameObject() = default;

            const length_vec& position() const;
            const velocity_vec& velocity() const;

            void setPosition(const length_vec&   pos);
            void setVelocity(const velocity_vec& vel);

            mass_t mass() const;
            void setMass(mass_t m);

            std::uint64_t id() const;
            std::uint64_t physics_id() const;

            void setID(std::uint64_t id);
            void setPhysicsID(std::uint64_t id);

        protected:
            std::uint64_t mID        = 0;
            std::uint64_t mPhysicsID = 0;
            mass_t mMass;

            length_vec   mPosition;
            velocity_vec mVelocity;
        };
    }
}

#endif //SOI_GAMEOBJECT_H
