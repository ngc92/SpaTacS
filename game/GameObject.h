//
// Created by erik on 9/28/16.
//

#ifndef SOI_GAMEOBJECT_H
#define SOI_GAMEOBJECT_H

#include <cstdint>
#include "physics/Object.h"
#include "game/ObjectID.h"

namespace spatacs
{
    namespace game
    {
        class GameObject
        {
        public:
            GameObject(ObjectType t);
            virtual ~GameObject() = default;

            const length_vec& position() const;
            const velocity_vec& velocity() const;

            void setPosition(const length_vec&   pos);
            void setVelocity(const velocity_vec& vel);

            mass_t mass() const;
            void setMass(mass_t m);

            ObjectID id() const;
            physics::ObjectID physics_id() const;

            void setID(ObjectID id);
            void setPhysicsID(physics::ObjectID id);

            virtual bool alive() const = 0;
            virtual void onStep() = 0;
            virtual GameObject* clone() const = 0;

            ObjectType type() const;

        protected:
            ObjectType    mType;
            ObjectID      mID;
            physics::ObjectID mPhysicsID;
            mass_t mMass;

            length_vec   mPosition;
            velocity_vec mVelocity;
        };

        length_t distance(const GameObject& s1, const GameObject& s2);
    }
}

#endif //SOI_GAMEOBJECT_H
