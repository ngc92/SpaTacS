//
// Created by erik on 12/31/16.
//

/// This file contains components that are used jointly by the game object entities
/// and the subcomponent entities.
/// \todo add tests!

#ifndef SPATACS_GAME_ENTITY_COMMON_COMPONENTS_H
#define SPATACS_GAME_ENTITY_COMMON_COMPONENTS_H


#include <string>
#include "physics/units.h"

namespace spatacs
{
namespace game
{
namespace components
{
    /*! Component that contains a textual identifier for the entity. The identifier
     * is intended to be used for user interaction. It might be translated, so never act
     * depending on the value here. Deliberately does not provide a way to change the name
     * after construction.
     */
    struct Name
    {
    public:
        explicit Name(std::string n = "") : mName(std::move(n)) {}

        const std::string& name() const { return mName; }

    private:
        std::string mName = "";
    };

    /*! A generic health component that saves a current and maximum health value.
     *  Provides some utility functions for getting/setting health and status.
     *  Invariant: 0 <= current <= maximum.
     */
    struct Health
    {
    public:
        explicit Health(double v = 0.0) : mCurrent(v), mMaximum(v) {}

        double current() const { return mCurrent; }
        double maximum() const { return mMaximum; }
        double status() const { return mCurrent / mMaximum; }

        /// sets the current health to \p new_health, clamped between 0 and maximum().
        /// \return the new health value.
        double setCurrent(double new_health);

        /// sets the maximum health. Clamps the current health to the new range.
        /// \throw std::logic_error, if \p new_maximum < 0.
        void setMaximum(double new_maximum);

        /// sets the health relative to maximum health.
        /// \return The new current health.
        double setHealthStatus(double percentage);

    private:
        double mCurrent = 0;
        double mMaximum = 0;
    };

    struct Timer
    {
        Timer() = default;

        explicit Timer(time_t t) : time(t) {}

        time_t time = 0.0_s;
    };

    class Activity
    {
    public:
        explicit Activity(double a = 1);

        double get() const;

        void set(double a);

    private:
        double mActivity;
    };
}
}
}
#endif //SPATACS_GAME_ENTITY_COMMON_COMPONENTS_H
