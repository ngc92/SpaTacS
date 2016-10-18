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

        class RequestFuel : public core::System<ComponentEntity, RequestFuel,
                core::Signature<FuelStorage>>
        {
        public:
            RequestFuel(const mass_t& request);
            mass_t provided() const { return mProvide; }
            void apply(const ComponentEntity& ety, FuelStorage& fs);
        private:
            mass_t mRequest;
            mass_t mProvide;
        };

        // -------------------------------------------------------------------------------------------
        //                                  Ship System handling
        // -------------------------------------------------------------------------------------------
        class Propulsion : public core::System<ComponentEntity, Propulsion,
                core::Signature<const EngineData, const Health>>
        {
        public:
            Propulsion(Starship& ship, accel_vec mDesiredAcceleration);
            void apply(const ComponentEntity& ce, const EngineData& engine, const Health& health);

            const accel_vec& getProduced() const { return mProducedAcceleration; }
            accel_t getMax() const { return mMaxAcceleration; }

        private:
            Starship& mShip;
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
    }
}

#endif //SPATACS_SYSTEMS_H
