#ifndef COMMANDS_H_INCLUDED
#define COMMANDS_H_INCLUDED

#include <boost/variant.hpp>

#include "Move.h"
#include "Attack.h"
#include "SetMode.h"

namespace spatacs
{
namespace cmd
{
	using Command = boost::variant<Move, Attack, SetWpnMode>;

	struct GetShip : public boost::static_visitor<std::uint64_t>
	{
		template<class T>
		std::uint64_t operator()(const T& t) const
		{
			return t.object();
		}
	};

    inline std::uint64_t get_ship( const Command& c )
    {
        return boost::apply_visitor(GetShip(), c);
    }

}
}

#endif // COMMANDS_H_INCLUDED
