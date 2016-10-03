//
// Created by erik on 9/27/16.
//

#ifndef SOI_EVENTS_H
#define SOI_EVENTS_H

#include <cstdint>
#include "physics/units.h"
#include "units.h"
#include <boost/variant/variant_fwd.hpp>

namespace spatacs
{
    namespace physics
    {
        class Object;

        namespace events
        {
            struct Spawn
            {
                //! \todo how do we handle ID assignment?
                length_vec   position;
                velocity_vec velocity;

                mass_t mass;
                length_t radius;

                std::uint64_t udata;

                time_t time;
            };

            struct Despawn
            {
                std::uint64_t target;
                time_t time;
            };

            struct ApplyForce
            {
                std::uint64_t target;
                force_vec force;          // kg km/s² = 1000 kg m/s² = kN
                time_t time;
            };

            struct Collision
            {
                std::uint64_t A;
                std::uint64_t B;
                time_t time;
            };

            using Event = boost::variant<Spawn, Despawn, ApplyForce, Collision>;

            // some helper functions
            //! \brief Checks whether the Event refers to an object with given \p id.
            //! \param id The ID for which to check.
            //! \return True, if the event references that id.
            bool has_target_id( std::uint64_t id, const Event& event );

            //! \brief Returns the time at which an event happens.
            //! \param event The event which to inspect.
            //! \return The time when \p event happens.
            time_t get_time(const Event &event);

            //! Constructs the object that is represented by the spawn event.
            //! \param spawn The event.
            //! \return An Object with all properties set according to spawn.
            Object get_object(const Spawn& spawn);
        }
    }
}

#endif //SOI_EVENTS_H