//
// Created by erik on 10/1/16.
//

#ifndef SOI_SETMODE_H
#define SOI_SETMODE_H

#include <cstdint>
#include <iosfwd>

namespace spatacs
{
namespace cmd
{
	class SetWpnMode
	{
	public:
        SetWpnMode( std::uint64_t object, std::uint64_t wp_id, std::uint64_t mode );
        SetWpnMode( std::istream& in );

		std::uint64_t object() const;
		std::uint64_t mode() const;
		std::uint64_t weapon() const;
	private:
		std::uint64_t mObject;
		std::uint64_t mMode;
        std::uint64_t mWeapon;
	};

	std::ostream& operator<<( std::ostream& out, const SetWpnMode& move );
}
}

#endif //SOI_SETMODE_H
