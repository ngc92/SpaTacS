#ifndef ATTACK_H_INCLUDED
#define ATTACK_H_INCLUDED

#include <cstdint>
#include <iosfwd>
#include <game/ObjectID.h>

namespace spatacs
{
namespace cmd
{
	class Attack
	{
	public:
		explicit Attack(game::ObjectID target);
		game::ObjectID target() const;
	private:
		game::ObjectID mTarget;
	};
	
	std::ostream& operator<<( std::ostream& out, const Attack& move );
}
}

#endif // ATTACK_H_INCLUDED
