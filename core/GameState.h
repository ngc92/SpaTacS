//
// Created by erik on 9/18/16.
//

#ifndef SOI_GAMESTATE_H
#define SOI_GAMESTATE_H

#include <vector>
#include <cstdint>

#include "Starship.h"
#include "Projectile.h"

namespace spatacs
{
namespace core
{
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
        std::vector<Starship>& getShips();
        const std::vector<Starship>& getShips() const;

        void addShip(Starship ship);

        // Projectiles
        void addProjectile( Projectile proj );
        std::vector<Projectile>& getProjectiles();
        const std::vector<Projectile>& getProjectiles() const;
        Projectile& getProjectile(std::uint64_t id);
        const Projectile& getProjectile( std::uint64_t id ) const;

        GameObject& getObject( std::uint64_t id );

        std::uint64_t getNextFreeID();
    private:
        std::vector<Starship> mStarships;
        std::vector<Projectile> mProjectiles;
        std::uint64_t mFreeID = 0;
    };
}
}

#endif //SOI_GAMESTATE_H
