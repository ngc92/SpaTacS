#ifndef MOVE_H_INCLUDED
#define MOVE_H_INCLUDED

#include <cstdint>
#include <vector>
#include "physics/units.h"
#include "ICommand.h"

namespace spatacs
{
namespace core
{
    class Starship;
}
namespace cmd
{
	class Move
	{
	public:
		Move(std::uint64_t object, length_t x, length_t y, length_t z, speed_t speed);
		Move(std::uint64_t object, length_vec p, speed_t speed);
		
		std::uint64_t object() const;
		std::size_t waypoint_count() const;
		const length_vec& target(std::size_t id = 0) const;
        speed_t speed() const;

        void addWaypoint( length_vec wp );

        accel_vec calcThrust(const core::Starship& ship) const;
	private:
		std::uint64_t mObject;
		std::vector<length_vec> mTargets;
		speed_t mSpeed;
	};
}
}

#endif // MOVE_H_INCLUDED
