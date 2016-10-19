//
// Created by erik on 9/27/16.
//

#include <boost/variant.hpp>
#include "Events.h"
#include "Object.h"

using namespace spatacs::physics;
using namespace events;

namespace
{

    struct HasTargetID : public boost::static_visitor<bool>
    {
        HasTargetID( std::uint64_t d) : id(d) {}
        std::uint64_t id;
        template<class T>
        bool operator()(const T& t) const {return false;}

        bool operator()(const Despawn& d) const { return d.target == id; }
        bool operator()(const ApplyForce& d) const { return d.target == id; }
        bool operator()(const Collision& d) const { return d.A == id || d.B == id; }
        bool operator()(const SetMass& d) const { return d.target == id; }
    };

    struct GetTime : public boost::static_visitor<spatacs::physics::time_t>
    {
        template<class T>
        auto operator()(const T& t) const {return t.time;}
    };
}

bool spatacs::physics::events::has_target_id(std::uint64_t id, const Event& event)
{
    return boost::apply_visitor(HasTargetID{id}, event);
}

spatacs::physics::time_t spatacs::physics::events::get_time(const Event &event)
{
    return boost::apply_visitor(GetTime{}, event);
}
