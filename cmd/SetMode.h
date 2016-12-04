//
// Created by erik on 10/1/16.
//

#ifndef SOI_SETMODE_H
#define SOI_SETMODE_H

#include <cstdint>
#include <iosfwd>
#include <string>
#include <game/ObjectID.h>
#include "game/systems/fwd.h"

namespace spatacs
{
namespace cmd
{
	class SetWpnMode
	{
	public:
        SetWpnMode(game::ObjectID object, game::CompEntID wp_id, std::string ammo);

		game::ObjectID object() const;
		const std::string& ammo() const;
		game::CompEntID weapon() const;
	private:
		game::ObjectID  mObject;
		std::string     mAmmo;
        game::CompEntID mWeapon;
	};

	class SetSystemActivity
	{
	public:
        SetSystemActivity(game::ObjectID object, game::CompEntID system_id, double activity);

		game::ObjectID object() const;
		double activity() const;
		game::CompEntID system() const;
	private:
		game::ObjectID  mObject;
		double          mActivity;
		game::CompEntID mSystem;
	};

	std::ostream& operator<<( std::ostream& out, const SetWpnMode& move );
}
}

#endif //SOI_SETMODE_H
