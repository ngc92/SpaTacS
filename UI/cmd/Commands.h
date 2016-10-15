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

}
}

#endif // COMMANDS_H_INCLUDED
