//
// Created by erik on 9/26/16.
//

#ifndef SOI_PHYSICSWORLD_H
#define SOI_PHYSICSWORLD_H

#include <cstdint>
#include <queue>
#include <map>
#include <functional>
#include "Object.h"
#include "units.h"

namespace spatacs
{
namespace physics
{
    struct ImpactInfo
    {
        std::uint64_t fixture_A;  //!< Userdata of the fixture of the first object.
        std::uint64_t fixture_B;  //!< Userdata of the fixture of the second object;
        length_vec touch_point;
        time_t time;
    };


    struct Collision
    {
        ObjectID A;        //!< ID of object A
        ObjectID B;        //!< ID of object B
        std::uint64_t fA;       //!< Userdata of fixture A
        std::uint64_t fB;       //!< Userdata of fixture B
        time_t time;
    };


    /*! \brief The main class of the physics simulation.
     *  \details This class is where the (physical) magic happens.
     *  \paragraph Data storage:
     *      Physic objects are stored inside a id -> physics object map.
     *      \todo maybe use a vector, and only a map of pointers.
     *  \paragraph Interaction with the GameThread:
     *      All saved objects can only be externally addressed as const refs.
     *      In order to change an object, a corresponding physics event has
     *      to be submitted. All these events are saved inside an event
     *      list which is ordered by time. The event "list" is implemented
     *      as a priority queue.
     *  \paragraph Collision handling:
     *      When the collisions are processed, we first determine all possible
     *      collisions. Then we process these events in order of their
     *      occurence, that is we call the user supplied callback function.
     */
    class PhysicsWorld
    {
    public:
        using collision_callback_fn = std::function<void(PhysicsWorld& world, const Object& A,
                                                         const Object& B, ImpactInfo info)>;

        const Object& getObject(ObjectID id) const;
        void setCollisionCallback( collision_callback_fn cb );

        /// spawn a new physics object.
        /// \return the ID that was assigned to the new object.
        ObjectID spawn(const Object& object);

        /// \return whether \p id existed and was despawned.
        bool despawn(ObjectID id);

        /// apply a force to the object with the given id.
        /// \attention note that the actual change in velocity
        /// can currently only happen at the end of a time step.
        void applyForce(ObjectID id, force_vec force);

        /// apply an impulse to the object with the given id.
        /// \note This changes the objects velocity immediately
        /// and causes collisions to be recalculated.
        void applyImpulse(ObjectID id, impulse_vec impulse);

        /// changes the mass of an object.
        /// Will affect only subsequent addForce calls.
        void setMass(ObjectID id, mass_t mass);

        /// completely update the object. Be careful not to disrupt the physics simulation with this!
        void updateObject(ObjectID id, length_vec new_position, velocity_vec new_veloctiy, mass_t new_mass);

        void simulate(time_t time_step);

    private:
        void pushEvent(Collision evt);

        // Event queue
        struct Compare
        {
            bool operator()(const Collision& a, const Collision& b) const;
        };
        using queue_t = std::priority_queue<Collision, std::vector<Collision>, Compare>;
        queue_t mEventQueue;

        // Objects
        ///! This struct saves the objects, and
        ///! all associated data used for processing
        struct ObjectRecord
        {
            Object       object;
            velocity_vec acceleration; // this is premultiplied with the correct dt
            time_t       update;
        };
        std::map<ObjectID, ObjectRecord> mObjects;
        std::uint64_t mFreeID = 1;

        const ObjectRecord& getObjectRec(ObjectID id) const;
        ObjectRecord& getObjectRec(ObjectID id);

        // Collision management
        collision_callback_fn mCollisionCallback;
        void detectCollisionsOf(ObjectID id, time_t max_dt, bool all = false);
        void filterCollisions(ObjectID id);

        time_t mNow = 0.0_s;
    };
}
}

#endif //SOI_PHYSICSWORLD_H
