//
// Created by erik on 10/18/16.
//

#ifndef SPATACS_SYSTEMS_H
#define SPATACS_SYSTEMS_H

#include <map>
#include "core/System.h"
#include "components.h"

namespace spatacs
{
    namespace game
    {
        class Starship;
        class EnergyManager;

        // General purpose components
        class TimerCountdown : public core::System<ComponentEntity, TimerCountdown, core::Signature<Timer>>
        {
        public:
            TimerCountdown(double dt);
            void apply(const ComponentEntity& ety, Timer& timer) const;
        private:
            double mDeltaT;
        };

        // -------------------------------------------------------------------------------------------
        //                                  Ship System handling
        // -------------------------------------------------------------------------------------------
        class Propulsion : public core::System<ComponentEntity, Propulsion,
                core::Signature<EngineData, const Health>>
        {
        public:
            Propulsion(Starship& ship, accel_vec mDesiredAcceleration);
            void apply(const ComponentEntity& ce, EngineData& engine, const Health& health);

            const accel_vec& getProduced() const { return mProducedAcceleration; }
            accel_t getMax() const { return mMaxAcceleration; }

        private:
            Starship& mShip;
            accel_vec mDesiredAcceleration;
            accel_vec mProducedAcceleration{.0, .0, .0};
            accel_t   mMaxAcceleration{.0};
        };

        class ShieldManagement : public core::System<ComponentEntity, ShieldManagement,
                core::Signature<ShieldGeneratorData, const Health>>
        {
        public:
            ShieldManagement(Starship& s, EnergyManager& emgr);
            void apply(const ComponentEntity& ety, ShieldGeneratorData& sgen, const Health& health);
        private:
            Starship& ship;
            EnergyManager& emgr;
        };

        class PowerProduction : public core::System<ComponentEntity, PowerProduction,
                core::Signature<const PowerPlantData, const Health>>
        {
        public:
            void apply(const ComponentEntity& ety, const PowerPlantData& pp, const Health& health);
            double energy() const { return mProducedEnergy; }
        private:
            double mProducedEnergy = 0;
        };

        class LifeSupportStep : public core::System<ComponentEntity, LifeSupportStep,
                core::Signature<LifeSupportData>>
        {
        public:
            LifeSupportStep(const Starship& s, EnergyManager& e);
            void apply(const ComponentEntity& ety, LifeSupportData& sup) const;
        private:
            const Starship& ship;
            EnergyManager& emgr;
        };

        class TankInfo : public core::System<const ComponentEntity, TankInfo, core::Signature<FuelStorage>>
        {
        public:
            TankInfo() = default;
            void apply(const ComponentEntity& ety, const FuelStorage& h);
            mass_t fuel() const     { return mFuel; }
            mass_t capacity() const { return mCapacity; }
        private:
            mass_t mFuel     = 0.0_kg;
            mass_t mCapacity = 0.0_kg;
        };

        class ListAmmunition : public core::System<const ComponentEntity, ListAmmunition,
                core::Signature<const AmmoStorage>>
        {
        public:
            void apply(const ComponentEntity& ety, const AmmoStorage& as);

            auto begin() const { return mCounts.begin();}
            auto end()   const { return mCounts.end(); }
        private:
            std::map<std::string, std::size_t> mCounts;
        };

        class Aiming : public core::System<ComponentEntity, Aiming,
                core::Signature<const WeaponAimData, const Health>>
        {
        public:
            Aiming(length_vec position, velocity_vec velocity);
            void apply(const game::ComponentEntity& ety, const game::WeaponAimData& aim,
                       const game::Health& health);

            const length_vec& aim_pos() const { return mAimPos; }
            const speed_t     speed() const { return mMuzzleVel; }
        private:
            length_vec mTargetPos;
            velocity_vec mTargetVel;

            length_vec mAimPos{.0, .0, .0};
            speed_t mMuzzleVel{.0};
        };
    }
}

#endif //SPATACS_SYSTEMS_H
