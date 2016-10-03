#ifndef MOVE_H_INCLUDED
#define MOVE_H_INCLUDED

#include <cstdint>
#include <iosfwd>
#include "physics/units.h"
#include "ICommand.h"

namespace spatacs
{
namespace cmd
{
	class Move : public ICommand
	{
	public:
		Move(std::uint64_t object, length_t x, length_t y, length_t z, speed_t speed);
		Move( std::istream& in );
		
		std::uint64_t object() const;
		const length_vec& target() const;
        speed_t speed() const;
	private:
		std::uint64_t mObject;
		length_vec mTarget;
		speed_t mSpeed;
	};
	
	std::ostream& operator<<( std::ostream& out, const Move& move );
}
}

#endif // MOVE_H_INCLUDED
