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
		Move() = default;
		Move(length_t x, length_t y, length_t z, speed_t speed);
		Move(length_vec p, speed_t speed);

		std::size_t waypoint_count() const;
		const length_vec& target(std::size_t id = 0) const;

        speed_t speed() const;
        Move& setSpeed( speed_t s );

        Move& addWaypoint(length_vec wp);

        accel_vec calcThrust(const core::Starship& ship) const;
	private:
		std::vector<length_vec> mTargets;
		speed_t mSpeed;
	};
}
}

#endif // MOVE_H_INCLUDED
