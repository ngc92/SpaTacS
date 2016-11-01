#ifndef ATTACK_H_INCLUDED
#define ATTACK_H_INCLUDED

#include <cstdint>
#include <iosfwd>

namespace spatacs
{
namespace cmd
{
	class Attack
	{
	public:
		Attack( std::uint64_t target );
		Attack( std::istream& in );

		std::uint64_t target() const;
	private:
		std::uint64_t mTarget;
	};
	
	std::ostream& operator<<( std::ostream& out, const Attack& move );
}
}

#endif // ATTACK_H_INCLUDED
