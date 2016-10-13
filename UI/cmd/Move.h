#ifndef MOVE_H_INCLUDED
#define MOVE_H_INCLUDED

#include <cstdint>
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
		const length_vec& target() const;
        speed_t speed() const;

        accel_vec calcThrust(const core::Starship& ship) const;
	private:
		std::uint64_t mObject;
		length_vec mTarget;
		speed_t mSpeed;
	};
}
}

#endif // MOVE_H_INCLUDED
