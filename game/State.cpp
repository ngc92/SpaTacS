//
// Created by erik on 1/2/17.
//

#include "State.h"
#include "entity/entity.h"

using namespace spatacs;
using namespace game;

// Storage classs
struct State::Storage
{
    GameEntityManager mManager;
};

// everything defaulted. We cannot do that inside the class, because the types are incomplete in the header.
State::State() = default;
State::~State() = default;
State::State(State&&) = default;

State::State(const State& o) : mStorage( std::make_unique<Storage>(*o.mStorage) )
{

}

std::unique_ptr<core::GameStateBase> State::clone() const
{
    // call the copy constructor explicitly here, because down in make_unique
    // it is private.
    return std::make_unique<State>(State(*this));
}

GameEntityManager& State::entity_manager()
{
    return mStorage->mManager;
}

const GameEntityManager& State::entity_manager() const
{
    return mStorage->mManager;
}

