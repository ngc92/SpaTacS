//
// Created by erik on 9/18/16.
//

#include <boost/throw_exception.hpp>
#include <stdexcept>
#include "game/GameState.h"
#include <algorithm>
#include "game/Starship.h"
#include "game/Projectile.h"

using namespace spatacs;
using namespace game;

GameState::GameState()
{

}

GameState::~GameState()
{

}

Starship& GameState::getShip(ObjectID id) {
    return dynamic_cast<Starship&>( getObject(id) );
}

const Starship& GameState::getShip(ObjectID id) const
{
    return dynamic_cast<const Starship&>( getObject(id) );
}


void GameState::add(std::unique_ptr<GameObject> obj)
{
    mObjects.push_back( obj.release() );
}

GameObject& GameState::getObject(ObjectID id)
{
    for(auto& p : mObjects)
        if(p.id() == id)
            return p;

    BOOST_THROW_EXCEPTION(std::logic_error("Could not find Object"));
}

const GameObject& GameState::getObject(ObjectID id) const
{
    for(auto& p : mObjects)
        if(p.id() == id)
            return p;

    BOOST_THROW_EXCEPTION(std::logic_error("Could not find Object"));
}

void GameState::cleanup()
{
    mObjects.erase_if( [](const GameObject& s) { return !s.alive(); });
}

const bool GameState::hasObject(ObjectID id) const
{
    for (auto& p : mObjects)
        if (p.id() == id)
            return true;
    return false;
}

std::unique_ptr<core::GameStateBase> GameState::clone() const
{
    return std::make_unique<GameState>(*this);
}
