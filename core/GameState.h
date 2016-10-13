//
// Created by erik on 9/18/16.
//

#ifndef SOI_GAMESTATE_H
#define SOI_GAMESTATE_H

#include <boost/ptr_container/ptr_vector.hpp>
#include <cstdint>

#include "GameObject.h"

namespace spatacs
{
namespace core
{
    inline GameObject* new_clone( const GameObject& s )
    {
        return s.clone();
    }

    class Starship;
    class Projectile;

    class GameState
    {
    public:
        GameState();
        ~GameState();
        GameState( GameState&& ) = default;
        explicit GameState( const GameState& ) = default;
        GameState& operator=(GameState&&) = default;
        GameState& operator=(const GameState&) = default;

        // Ships
        Starship& getShip( const std::uint64_t id );
        const Starship& getShip( const std::uint64_t id ) const;
        void add(std::unique_ptr<Starship> ship);

        // Projectiles
        void add(std::unique_ptr<Projectile> proj);
        Projectile& getProjectile(std::uint64_t id);
        const Projectile& getProjectile( std::uint64_t id ) const;

        // Objccts
        GameObject& getObject( std::uint64_t id );
        const GameObject& getObject( std::uint64_t id ) const;
        const bool hasObject( std::uint64_t id ) const;

        auto begin() { return mObjects.begin(); };
        auto begin() const { return mObjects.begin(); };
        auto end() { return mObjects.end(); };
        auto end() const { return mObjects.end(); };

        /// removes all objects that are not alive
        void cleanup();

        std::uint64_t getNextFreeID();
    private:
        boost::ptr_vector<GameObject> mObjects;

        std::uint64_t mFreeID = 0;
    };
}
}

#endif //SOI_GAMESTATE_H
