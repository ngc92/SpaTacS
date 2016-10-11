//
// Created by erik on 9/18/16.
//

#include <boost/throw_exception.hpp>
#include <stdexcept>
#include "GameState.h"
#include <algorithm>
#include "Starship.h"
#include "Projectile.h"

using namespace spatacs;
using namespace core;

GameState::GameState()
{

}

GameState::~GameState()
{

}

Starship& GameState::getShip(const std::uint64_t id) {
    return dynamic_cast<Starship&>( getObject(id) );
}

const Starship& GameState::getShip(const std::uint64_t id) const
{
    return dynamic_cast<const Starship&>( getObject(id) );
}

void GameState::add(std::unique_ptr<Starship> ship) {
    /// \todo ensure uniqueness of ids
    mObjects.push_back( ship.release() );
}


void GameState::add(std::unique_ptr<Projectile> proj)
{
    mObjects.push_back( proj.release() );
}

Projectile& GameState::getProjectile(std::uint64_t id)
{
    return dynamic_cast<Projectile&>( getObject(id) );
}

const Projectile& GameState::getProjectile(std::uint64_t id) const
{
    return dynamic_cast<const Projectile&>( getObject(id) );
}

GameObject& GameState::getObject(std::uint64_t id)
{
    for(auto& p : mObjects)
        if(p.id() == id)
            return p;

    BOOST_THROW_EXCEPTION(std::logic_error("Could not find Object"));
}

const GameObject& GameState::getObject(std::uint64_t id) const
{
    for(auto& p : mObjects)
        if(p.id() == id)
            return p;

    BOOST_THROW_EXCEPTION(std::logic_error("Could not find Object"));
}

std::uint64_t GameState::getNextFreeID()
{
    return ++mFreeID;
}

void GameState::cleanup()
{
    mObjects.erase_if( [](const GameObject& s) { return !s.alive(); });
}
