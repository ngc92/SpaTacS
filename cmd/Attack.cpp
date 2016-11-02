#include "Attack.h"
#include <iostream>
#include <game/ObjectID.h>

namespace spatacs
{
namespace cmd
{
	Attack::Attack(game::ObjectID target):
		mTarget( target )
	{
	}
	
	game::ObjectID Attack::target() const
	{
		return mTarget;
	}
	
	std::ostream& operator<<( std::ostream& out, const Attack& m )
	{
		return out << "attack["<< m.target() << "]";
	}
}
}
