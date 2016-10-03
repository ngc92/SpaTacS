//
// Created by erik on 9/18/16.
//

#include <boost/throw_exception.hpp>
#include <stdexcept>
#include "GameState.h"
#include <algorithm>

using namespace spatacs;
using namespace core;

GameState::GameState()
{

}

GameState::~GameState()
{

}

Starship& GameState::getShip(const std::uint64_t id) {
    for(auto& s : mStarships)
    {
        if(s.id() == id)
            return s;
    }

    BOOST_THROW_EXCEPTION(std::logic_error("Could not find Ship ID"));
}

const Starship& GameState::getShip(const std::uint64_t id) const
{
    for(auto& s : mStarships)
    {
        if(s.id() == id)
            return s;
    }

    BOOST_THROW_EXCEPTION(std::logic_error("Could not find Ship ID"));
}

std::vector<Starship>& GameState::getShips()
{
    return mStarships;
}

const std::vector<Starship>& GameState::getShips() const
{
    return mStarships;
}
void GameState::addShip(Starship ship) {
    /// \todo ensure uniqueness of ids
    mStarships.push_back( std::move(ship) );
}


void GameState::addProjectile(Projectile proj)
{
    mProjectiles.push_back( std::move(proj) );
}

std::vector<Projectile>& GameState::getProjectiles()
{
    return mProjectiles;
}

const std::vector<Projectile>& GameState::getProjectiles() const
{
    return mProjectiles;
}

Projectile& GameState::getProjectile(std::uint64_t id)
{
    for(auto& p : mProjectiles)
        if(p.id() == id)
            return p;

    BOOST_THROW_EXCEPTION( std::logic_error("Projectile not found") );
}

const Projectile& GameState::getProjectile(std::uint64_t id) const
{
    for(auto& p : mProjectiles)
        if(p.id() == id)
            return p;

    BOOST_THROW_EXCEPTION( std::logic_error("Projectile not found") );
}

GameObject& GameState::getObject(std::uint64_t id)
{
    for(auto& p : mProjectiles)
        if(p.id() == id)
            return p;
    for(auto& s : mStarships)
        if(s.id() == id)
            return s;


    BOOST_THROW_EXCEPTION(std::logic_error("Could not find Object"));
}

std::uint64_t GameState::getNextFreeID()
{
    return ++mFreeID;
}