#ifndef MOVE_H_INCLUDED
#define MOVE_H_INCLUDED

#include <cstdint>
#include <vector>
#include <memory>
#include <game/ObjectID.h>
#include "physics/units.h"
#include "ICommand.h"

namespace spatacs
{
namespace core
{
    class GameState;
}
namespace game
{
    class Starship;
}
namespace cmd
{
    namespace movement
    {
        class MvCommand
        {
        public:
            virtual ~MvCommand() = default;
            virtual accel_vec thrust(const game::Starship& ship) const = 0;
            virtual void update(const game::Starship& ship, const core::GameState& state) = 0;
            virtual std::size_t point_count() const = 0;
            virtual const length_vec& target(std::size_t id = 0) const = 0;
        };
    }
    class Move
    {
    public:
        Move() = default;
        Move(std::unique_ptr<movement::MvCommand> c);

        // this is for movement control
        accel_vec thrust(const game::Starship& ship) const;
        void update(const game::Starship& ship, const core::GameState& state);

        // this is for drawing
        std::size_t point_count() const;
        const length_vec& target(std::size_t id = 0) const;

        explicit operator bool() const;
        movement::MvCommand& getCommand();
        const movement::MvCommand& getCommand() const;

    private:
        std::unique_ptr<movement::MvCommand> mCommand;
    };

    namespace movement {
        class FollowRoute : public MvCommand
        {
        public:
            FollowRoute(length_vec p, speed_t speed);

            std::size_t point_count() const;
            const length_vec& target(std::size_t id = 0) const;

            speed_t speed() const;
            FollowRoute& setSpeed(speed_t s);
            FollowRoute& addWaypoint(length_vec wp);

            accel_vec thrust(const game::Starship& ship) const override;
            void update(const game::Starship& ship, const core::GameState& state) override;

        private:
            std::vector<length_vec> mTargets;
            speed_t mSpeed;
        };

        class EngageTarget : public MvCommand
        {
        public:
            EngageTarget(game::ObjectID target_id, speed_t speed);

            std::size_t point_count() const;
            const length_vec& target(std::size_t id = 0) const;

            speed_t speed() const;
            EngageTarget& setSpeed(speed_t s);

            accel_vec thrust(const game::Starship& ship) const override;
            void update(const game::Starship& ship, const core::GameState& state) override;

        private:
            game::ObjectID mTargetID;
            length_vec mTarget;
            velocity_vec mTargetVelocity;
            speed_t mSpeed;
            length_t mDistance = 1.0_km;
        };
    }
}
}

#endif // MOVE_H_INCLUDED
