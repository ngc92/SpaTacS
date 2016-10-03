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
            length_t radius() const;
            mass_t mass() const;
            std::uint64_t id() const;
            std::uint64_t physics_id() const;

            void setID(std::uint64_t id);
            physics::Object& getPhysicsObject();
        private:
            physics::Object mPhysicsObj;
        };
    }
}

#endif //SOI_GAMEOBJECT_H
