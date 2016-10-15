#include "Move.h"
#include <iostream>
#include <core/components/Engine.h>
#include <core/Starship.h>
#include <events/Accelerate.h>

namespace spatacs
{
namespace cmd
{
	Move::Move(length_t x, length_t y, length_t z, speed_t speed) :
            Move(length_vec{x, y, z}, speed)
	{
	}

    const length_vec& Move::target(std::size_t id) const
	{
		return mTargets.at(id);
	}

	speed_t Move::speed() const
	{
		return mSpeed;
	}

    Move::Move(length_vec p, speed_t speed) :
		mTargets({p}), mSpeed(speed)
    {

    }

    accel_vec Move::calcThrust(const core::Starship& ship) const
    {
        length_vec target_pos = ship.position();
        if( !mTargets.empty() )
        {
            target_pos = mTargets.front();
        }

        // and handle the command
        physics::time_t time_to_brake = length(ship.velocity()) / (ship.engine().max_thrust() / ship.mass());

        if( length(target_pos - ship.position()) < 0.5_km && mTargets.size() > 0)
        {
            const_cast<Move*>(this)->mTargets.erase(mTargets.begin());
        }

        auto delta = (target_pos - (ship.position() + time_to_brake * ship.velocity())) / 1.0_s;
        auto l = length(delta);
        if(l > speed())
            delta *= double(speed() / l);


        /// \todo this looks fishy!
        auto dv = (delta - ship.velocity()) / 1.0_s;
        return dv;
    }

    std::size_t Move::waypoint_count() const
    {
        return mTargets.size();
    }

    Move& Move::addWaypoint(length_vec wp)
    {
        mTargets.push_back(wp);
        return *this;
    }

    Move& Move::setSpeed(speed_t s)
    {
        mSpeed = s;
        return *this;
    }
}
}
