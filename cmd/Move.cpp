#include "Move.h"
#include <iostream>
#include <game/Starship.h>
#include <events/Accelerate.h>
#include "game/GameState.h"

using namespace spatacs;
using namespace cmd::movement;

const length_vec& FollowRoute::target(std::size_t id) const
{
    return mTargets.at(id);
}

speed_t FollowRoute::speed() const
{
    return mSpeed;
}

FollowRoute::FollowRoute(length_vec p, speed_t speed) :
    mTargets({p}), mSpeed(speed)
{

}

accel_vec FollowRoute::thrust(const game::Starship& ship) const
{
    length_vec target_pos = ship.position();
    if( !mTargets.empty() )
    {
        target_pos = mTargets.front();
    }

    // and handle the command
    auto max_accel = ship.getMaxAcceleration();
    if(max_accel.value == 0.0)
        max_accel.value = 1.0;

    auto ship_speed = length(ship.velocity());
    physics::time_t time_to_brake = ship_speed / max_accel;
    if(mTargets.size() > 1)
        time_to_brake *= 0.25;

    auto delta = (target_pos - (ship.position() + time_to_brake * ship.velocity())) / 1.0_s;
    auto l = length(delta);
    if(l > speed())
        delta *= double(speed() / l);


    /// \todo this looks fishy!
    auto dv = (delta - ship.velocity()) / 1.0_s;
    return dv;
}

void FollowRoute::update(const game::Starship& ship, const game::GameState& state)
{
    if( mTargets.empty() )
        return;

    auto target_pos = mTargets.front();
    auto ship_speed = length(ship.velocity());
    if( length(target_pos - ship.position()) < 0.3_km + 0.1_s*ship_speed && mTargets.size() > 0)
    {
        mTargets.erase(mTargets.begin());
    }
}

std::size_t FollowRoute::point_count() const
{
    return mTargets.size();
}

FollowRoute& FollowRoute::addWaypoint(length_vec wp)
{
    mTargets.push_back(wp);
    return *this;
}

FollowRoute& FollowRoute::setSpeed(speed_t s)
{
    mSpeed = s;
    return *this;
}

// -----------------------------------------------------------------------------------------

cmd::Move::Move(std::unique_ptr<MvCommand> c) : mCommand(std::move(c))
{
}

accel_vec cmd::Move::thrust(const game::Starship& ship) const
{
    if(mCommand)
        return mCommand->thrust(ship);
    return accel_vec(0, 0, 0);
}

void cmd::Move::update(const game::Starship& ship, const game::GameState& state)
{
    if(mCommand)
        return mCommand->update(ship, state);
}

std::size_t cmd::Move::point_count() const
{
    if(mCommand)
        return mCommand->point_count();
    return 0;
}

const length_vec& cmd::Move::target(size_t id) const
{
    if(mCommand)
        return mCommand->target(id);
    BOOST_THROW_EXCEPTION(std::logic_error("Trying to get target, but none set."));
}

cmd::movement::MvCommand& cmd::Move::getCommand()
{
    if(!mCommand)
        BOOST_THROW_EXCEPTION(std::logic_error("Trying to get command, but none set."));
    return *mCommand;
}
const cmd::movement::MvCommand& cmd::Move::getCommand() const
{
    if(!mCommand)
        BOOST_THROW_EXCEPTION(std::logic_error("Trying to get command, but none set."));
    return *mCommand;
}

cmd::Move::operator bool() const
{
    return (bool)mCommand;
}

std::size_t EngageTarget::point_count() const
{
    return 1;
}

const length_vec& EngageTarget::target(size_t id) const
{
    return mTarget;
}

speed_t EngageTarget::speed() const
{
    return mSpeed;
}

EngageTarget& EngageTarget::setSpeed(speed_t s)
{
    mSpeed = s;
    return *this;
}

accel_vec EngageTarget::thrust(const game::Starship& ship) const
{
    // and handle the command
    auto max_accel = ship.getMaxAcceleration();
    if(max_accel.value == 0.0)
        max_accel.value = 1.0;

    auto ship_speed = length(ship.velocity());
    physics::time_t time_to_brake = ship_speed / max_accel;
    auto future_target = mTarget + mTargetVelocity * length(mTarget - ship.position()) / speed();
    auto delta = (future_target - (ship.position() + time_to_brake * ship.velocity())) / 1.0_s;
    auto l = length(delta);
    if(l > speed())
        delta *= double(speed() / l);


    /// \todo this looks fishy!
    auto dv = (delta - ship.velocity()) / 1.0_s;
    return dv;
}

EngageTarget::EngageTarget(game::ObjectID target_id, speed_t speed) :
        mTargetID(target_id), mSpeed(speed)
{

}

void EngageTarget::update(const game::Starship& ship, const game::GameState& state)
{
    if(!mTargetID)
        return;

    if(state.hasObject(mTargetID))
    {
        auto& target = state.getObject(mTargetID);
        mTarget = target.position();
        auto delta = ship.position() - mTarget;
        mTarget += delta / length(delta) * mDistance;
        mTargetVelocity = target.velocity();
    } else
    {
        mTargetID.reset();
    }
}
