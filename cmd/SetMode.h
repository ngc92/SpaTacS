//
// Created by erik on 10/1/16.
//

#ifndef SOI_SETMODE_H
#define SOI_SETMODE_H

#include <cstdint>
#include <iosfwd>
#include <string>
#include <game/ObjectID.h>

namespace spatacs
{
namespace cmd
{
	class SetWpnMode
	{
	public:
        SetWpnMode(game::ObjectID object, std::uint64_t wp_id, std::string ammo);

		game::ObjectID object() const;
		const std::string& ammo() const;
		std::uint64_t weapon() const;
	private:
		game::ObjectID mObject;
		std::string    mAmmo;
        std::uint64_t  mWeapon;
	};

	std::ostream& operator<<( std::ostream& out, const SetWpnMode& move );
}
}

#endif //SOI_SETMODE_H
