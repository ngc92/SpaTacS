//
// Created by erik on 10/10/16.
//

#ifndef SPATACS_FIXTURE_H
#define SPATACS_FIXTURE_H

#include "units.h"

namespace spatacs
{
namespace physics
{
    class Fixture
    {
    public:
        Fixture(std::uint64_t parent, length_t rad);

        // shape data
        length_t radius() const;
        void setRadius(length_t rad);

        // userdata
        std::uint64_t userdata() const;
        void setUserdata(std::uint64_t value);

        // parent
        std::uint64_t parent() const;
        void setParent( std::uint64_t );

    private:
        length_t mRadius{0.01f};
        std::uint64_t mParent; // parent object
        std::uint64_t mUserData;
    };
}
}

#endif //SPATACS_FIXTURE_H
