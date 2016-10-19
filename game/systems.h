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
                core::Signature<const EngineData, const Health, FuelRequest>>
        {
        public:
            Propulsion(const Starship& ship, accel_vec mDesiredAcceleration);
            void apply(const ComponentEntity& ce, const EngineData& engine, const Health& health, FuelRequest&);

            const accel_vec& getProduced() const { return mProducedAcceleration; }
            accel_t getMax() const { return mMaxAcceleration; }

        private:
            const Starship& mShip;
            accel_vec mDesiredAcceleration;
            accel_vec mProducedAcceleration{.0, .0, .0};
            accel_t   mMaxAcceleration{.0};
        };

        class ShieldManagement : public core::System<ComponentEntity, ShieldManagement,
                core::Signature<ShieldGeneratorData, EnergyManagement, const Health>>
        {
        public:
            ShieldManagement(Starship& s);
            void apply(const ComponentEntity& ety, ShieldGeneratorData& sgen,
                       EnergyManagement& egy, const Health& health);
        private:
            Starship& ship;
        };

        struct PowerProduction : public core::System<ComponentEntity, PowerProduction,
                core::Signature<PowerPlantData, EnergyManagement, const Health>>
        {
            void apply(const ComponentEntity& ety, PowerPlantData& pp,
                       EnergyManagement& egy, const Health& health) const;
        };

        class LifeSupportStep : public core::System<ComponentEntity, LifeSupportStep,
                core::Signature<LifeSupportData, EnergyManagement>>
        {
        public:
            LifeSupportStep(const Starship& s);
            void apply(const ComponentEntity& ety, LifeSupportData& sup, EnergyManagement& egy) const;
        private:
            const Starship& ship;
        };


        // Fuel subsystems
        class FuelDistribution : public core::System<game::ComponentEntity, FuelDistribution, core::Signature<game::FuelRequest>>
        {
        public:
            FuelDistribution(mass_t available);
            void apply(const game::ComponentEntity& ety, game::FuelRequest& h);
            mass_t fuel() const     { return mFuel; }
        private:
            mass_t mFuel     = 0.0_kg;
        };

        class FuelConsumption : public core::System<game::ComponentEntity, FuelConsumption, core::Signature<game::FuelStorage>>
        {
        public:
            FuelConsumption(mass_t consume);
            void apply(const game::ComponentEntity& ety, game::FuelStorage& h);
        private:
            mass_t mConsume = 0.0_kg;
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
