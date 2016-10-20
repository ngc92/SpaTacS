//
// Created by erik on 10/18/16.
//

#ifndef SPATACS_SYSTEMS_H
#define SPATACS_SYSTEMS_H

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

        class TankInfo : public core::System<const game::ComponentEntity, TankInfo, core::Signature<game::FuelStorage>>
        {
        public:
            TankInfo() = default;
            void apply(const game::ComponentEntity& ety, const game::FuelStorage& h);
            mass_t fuel() const     { return mFuel; }
            mass_t capacity() const { return mCapacity; }
        private:
            mass_t mFuel     = 0.0_kg;
            mass_t mCapacity = 0.0_kg;
        };
    }
}

#endif //SPATACS_SYSTEMS_H
