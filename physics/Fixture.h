//
// Created by erik on 10/10/16.
//

#ifndef SPATACS_FIXTURE_H
#define SPATACS_FIXTURE_H

#include "units.h"
#include "ObjectID.h"

namespace spatacs
{
namespace physics
{
    class Fixture
    {
    public:
        Fixture(ObjectID parent, length_t rad);

        // shape data
        length_t radius() const;
        void setRadius(length_t rad);

        // userdata
        std::uint64_t userdata() const;
        void setUserdata(std::uint64_t value);

        // parent
        ObjectID parent() const;
        void setParent( ObjectID parent );

    private:
        length_t mRadius{0.01f};
        ObjectID mParent; // parent object
        std::uint64_t mUserData;
    };
}
}

#endif //SPATACS_FIXTURE_H
