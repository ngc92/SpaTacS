//
// Created by erik on 9/21/16.
//

#ifndef SOI_SPAWN_H
#define SOI_SPAWN_H

#include "IEvent.h"
#include <memory>
#include "game/Damage.h"

namespace spatacs
{
namespace events
{
    class SpawnProjectile : public IEvent
    {
    public:
        SpawnProjectile(std::uint64_t shooter, length_vec pos, velocity_vec vel, mass_t mass, length_t rad,
                                game::Damage dmg);

        virtual void apply(EventContext& context) const override;
    private:
        std::uint64_t mShooter;
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
    private:
        std::uint64_t mTeam;
        std::string mName;
        std::string mType;
        length_vec mPosition;
        struct AmmoData{
            AmmoData(const std::string& type, size_t amount);

            std::string type; std::size_t amount; };
        std::vector<AmmoData> mAmmo; /// \todo this is surely not a good way to pass that information
    };
}
}

#endif //SOI_SPAWN_H
