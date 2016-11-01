#include "Attack.h"
#include <iostream>

namespace spatacs
{
namespace cmd
{
	Attack::Attack( std::uint64_t target ):
		mTarget( target )
	{
	}
	
	Attack from_stream( std::istream& in );
	
	Attack::Attack( std::istream& in ) : Attack( from_stream( in ) )
	{
		
	}
	
	std::uint64_t Attack::target() const
	{
		return mTarget;
	}
	
	
	Attack from_stream( std::istream& in )
	{
		std::uint64_t t;
		in >> t;
		return Attack(t);
	}
	
	std::ostream& operator<<( std::ostream& out, const Attack& m )
	{
		return out << "attack["<< m.target() << "]";
	}
}
}
