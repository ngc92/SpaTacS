//
// Created by erik on 9/18/16.
//

#ifndef SOI_GAMESTATE_H
#define SOI_GAMESTATE_H

#include <boost/ptr_container/ptr_vector.hpp>
#include <cstdint>

#include "GameObject.h"
#include "core/GameStateBase.h"

namespace spatacs
{
namespace game
{
    class Starship;
    class Projectile;
    inline game::GameObject* new_clone( const game::GameObject& s )
    {
        return s.clone();
    }

    class GameState : public core::GameStateBase
    {
        using GameObject = game::GameObject;
        using Starship   = game::Starship;
        using Projectile = game::Projectile;
        std::uint64_t mFreeID = 0;
    public:
        GameState();
        ~GameState();
        GameState( GameState&& ) = default;
        explicit GameState( const GameState& ) = default;
        GameState& operator=(GameState&&) = default;
        GameState& operator=(const GameState&) = default;

        // Ships
        Starship& getShip(game::ObjectID id);
        const Starship& getShip(game::ObjectID id) const;

        // Objects
        void add(std::unique_ptr<GameObject> obj);
        GameObject& getObject(game::ObjectID id);
        const GameObject& getObject(game::ObjectID id) const;
        const bool hasObject(game::ObjectID id) const;

        auto begin() { return mObjects.begin(); };
        auto begin() const { return mObjects.begin(); };
        auto end() { return mObjects.end(); };
        auto end() const { return mObjects.end(); };

        /// removes all objects that are not alive
        void cleanup() override;

        std::unique_ptr<GameStateBase> clone() const override;

        uint64_t getNextFreeID();

    private:
        boost::ptr_vector<GameObject> mObjects;

    };
}
}

#endif //SOI_GAMESTATE_H
