#include "Move.h"
#include <iostream>
#include <core/components/Engine.h>
#include <core/Starship.h>
#include <events/Accelerate.h>

namespace spatacs
{
namespace cmd
{
	Move::Move(std::uint64_t object, length_t x, length_t y, length_t z, speed_t speed):
		mObject( object ), mTarget( {x, y, z} ), mSpeed(speed)
	{
	}
	
	namespace
	{
		Move from_stream( std::istream& in );
	}
	
	Move::Move( std::istream& in ) : Move( from_stream( in ) )
	{
		
	}
		
	std::uint64_t Move::object() const
	{
		return mObject;
	}

	const length_vec& Move::target() const
	{
		return mTarget;
	}

	speed_t Move::speed() const
	{
		return mSpeed;
	}

    Move::Move(std::uint64_t object, length_vec p, speed_t speed) :
		mObject(object), mTarget(p), mSpeed(speed)
    {

    }

    accel_vec Move::calcThrust(const core::Starship& ship) const
    {
        assert(ship.id() == object());

        // and handle the command
        physics::time_t time_to_brake = length(ship.velocity()) / (ship.engine().max_thrust() / ship.mass());

        auto delta = (target() - (ship.position() + time_to_brake * ship.velocity())) / 1.0_s;
        auto l = length(delta);
        if(l > speed())
            delta *= double(speed() / l);

        /// \todo this looks fishy!
        auto dv = (delta - ship.velocity()) / 1.0_s;
        return dv;
    }

    namespace
	{
		Move from_stream( std::istream& in )
		{
			std::uint64_t o;
			double x, y, z, s;
			in >> o >> x >> y >> z >> s;
			return Move(o, x*1.0_km, y*1.0_km, z*1.0_km, s*1.0_km / 1.0_s);
		}
	}
	
	std::ostream& operator<<( std::ostream& out, const Move& m )
	{
		return out << "move["<< m.object() <<", " <<  m.target() << ", " << m.speed() << "]";
	}
}
}
