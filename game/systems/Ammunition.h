//
// Created by erik on 12/3/16.
//

#ifndef SPATACS_GAME_AMMUNITION_H
#define SPATACS_GAME_AMMUNITION_H

#include "fwd.h"
#include <vector>
#include "../components.h"

namespace spatacs
{
namespace game
{
namespace systems
{
    namespace signatures
    {
        using ListAmmoSignature   = core::ecs::Signature<const AmmoStorage>;
        using ChangeAmmoSignature = core::ecs::Signature<AmmoStorage>;
    }

    class ListAmmunition : public core::ecs::System<signatures::ListAmmoSignature>
    {
    public:
        void operator()(const AmmoStorage& as);

        std::size_t capacity() const { return mCapacity; }
        auto begin() const { return mAmmos.begin();}
        auto end()   const { return mAmmos.end(); }
        const std::vector<AmmoStorage::Ammo>& getAmmos() const { return mAmmos; }
    private:
        std::size_t mCapacity = 0;
        std::vector<AmmoStorage::Ammo> mAmmos;
    };

    class AddAmmunition : public core::ecs::System<signatures::ChangeAmmoSignature>
    {
    public:
        AddAmmunition(const AmmoData& a, std::size_t amount);
        void operator()(AmmoStorage& as);
    private:
        AmmoData mData;
        std::size_t mAmount;
    };

    class GetAmmunition : public core::ecs::System<signatures::ChangeAmmoSignature>
    {
    public:
        GetAmmunition(std::string type);
        void operator()(AmmoStorage& as);

        bool has() const;
        const AmmoData& ammo_data() const;
    private:
        std::string mType;
        const AmmoData* mData;
    };
}
}
}

#endif //SPATACS_GAME_AMMUNITION_H
