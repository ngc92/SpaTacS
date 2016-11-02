//
// Created by erik on 9/21/16.
//

#ifndef SPATACS_SPAWN_H
#define SPATACS_SPAWN_H

#include "IEvent.h"
#include <memory>
#include "game/ObjectID.h"
#include "game/Damage.h"

namespace spatacs
{
namespace events
{
    class SpawnProjectile : public IEvent
    {
    public:
        SpawnProjectile(game::ObjectID shooter, length_vec pos,
                        velocity_vec vel, mass_t mass, length_t rad,
                        game::Damage dmg);

        virtual void apply(EventContext& context) const override;
    private:
        game::ObjectID mShooter;
        length_vec    mPosition;
        velocity_vec  mVelocity;
        mass_t        mMass;
        length_t      mRadius;
        game::Damage  mDamage;
    };

    class SpawnShip : public IEvent
    {
    public:
        SpawnShip(std::uint64_t team, std::string name, std::string type, const length_vec& position);
        virtual void apply(EventContext& context) const override;
        void addAmmunition(std::string name, std::size_t amount);
        void setFuel(mass_t f);
    private:
        std::uint64_t mTeam;
        std::string mName;
        std::string mType;
        length_vec mPosition;

        mass_t mFuel = 0.0_kg;
        struct AmmoData
        {
            AmmoData(const std::string& type, size_t amount);
            std::string type; std::size_t amount;
        };
        std::vector<AmmoData> mAmmo;
    };
}
}

#endif //SPATACS_SPAWN_H
