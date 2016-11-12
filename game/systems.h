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
    namespace core
    {
        template<class T>
        class EntityManager;
    }
    namespace game
    {
        class Starship;
        class EnergyManager;

        // General purpose components
        class TimerCountdown : public core::System<ComponentEntity, TimerCountdown, core::Signature<Timer>>
        {
        public:
            TimerCountdown(double dt);
            void apply(Timer& timer) const;
        private:
            double mDeltaT;
        };

        // -------------------------------------------------------------------------------------------
        //                                  Ship System handling
        // -------------------------------------------------------------------------------------------
        class Propulsion : public core::System<ComponentEntity, Propulsion,
                core::Signature<EngineData, const Health, const Activity>>
        {
        public:
            Propulsion(Starship& ship, accel_vec mDesiredAcceleration);
            void apply(EngineData& engine, const Health& health, const Activity& acc);

            const accel_vec& getProduced() const { return mProducedAcceleration; }
            accel_t getMax() const { return mMaxAcceleration; }

        private:
            Starship& mShip;
            accel_vec mDesiredAcceleration;
            accel_vec mProducedAcceleration{.0, .0, .0};
            accel_t   mMaxAcceleration{.0};
        };

        class ShieldManagement : public core::System<ComponentEntity, ShieldManagement,
                core::Signature<ShieldGeneratorData, const Health, const Activity>>
        {
        public:
            ShieldManagement(Starship& s, EnergyManager& emgr);
            void apply(ShieldGeneratorData& sgen, const Health& health, const Activity& acc);
        private:
            Starship& ship;
            EnergyManager& emgr;
        };

        class PowerProduction : public core::System<ComponentEntity, PowerProduction,
                core::Signature<const PowerPlantData, const Health, const Activity>>
        {
        public:
            PowerProduction(core::EntityManager <ComponentEntity>& entities);
            void apply(const PowerPlantData& pp, const Health& health, const Activity& acc);
            energy_t energy() const { return mProducedEnergy; }
        private:
            core::EntityManager<ComponentEntity>& mEntites;
            energy_t mProducedEnergy = 0.0_J;
        };

        class LifeSupportStep : public core::System<ComponentEntity, LifeSupportStep,
                core::Signature<LifeSupportData>>
        {
        public:
            LifeSupportStep(const Starship& s, EnergyManager& e);
            void apply(LifeSupportData& sup) const;
        private:
            const Starship& ship;
            EnergyManager& emgr;
        };

        class TankInfo : public core::System<const ComponentEntity, TankInfo, core::Signature<FuelStorage>>
        {
        public:
            TankInfo() = default;
            void apply(const FuelStorage& h);
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
            void apply(const AmmoStorage& as);

            std::size_t capacity() const { return mCapacity; }
            auto begin() const { return mAmmos.begin();}
            auto end()   const { return mAmmos.end(); }
            const std::vector<AmmoStorage::Ammo>& getAmmos() const { return mAmmos; }
        private:
            std::size_t mCapacity = 0;
            std::vector<AmmoStorage::Ammo> mAmmos;
        };

        class AddAmmunition : public core::System<ComponentEntity, AddAmmunition,
                core::Signature<AmmoStorage>>
        {
        public:
            AddAmmunition(const AmmoData& a, std::size_t amount) : mData(a), mAmount(amount) { };
            void apply(AmmoStorage& as);
        private:
            AmmoData mData;
            std::size_t mAmount;
        };

        class Aiming : public core::System<ComponentEntity, Aiming,
                core::Signature<const WeaponAimData, const Health>>
        {
        public:
            Aiming(length_vec position, velocity_vec velocity);
            void apply(const game::WeaponAimData& aim, const game::Health& health);

            const length_vec& aim_pos() const { return mAimPos; }
            const speed_t     speed() const { return mMuzzleVel; }
        private:
            length_vec mTargetPos;
            velocity_vec mTargetVel;

            length_vec mAimPos{.0, .0, .0};
            speed_t mMuzzleVel{.0};
        };

        template<class Lambda>
        auto for_each_weapon_id(Lambda&& lambda)
        {
            return core::make_system<
                    const game::ComponentEntity, const game::ComponentEntity, const game::WeaponAimData>(
                    [l=std::move(lambda)](const game::ComponentEntity& ety, const game::WeaponAimData&){
                        l(ety.id());
                    }
            );
        }

        mass_t getFuel(core::EntityManager<ComponentEntity>& entities, mass_t request);

    }
}

#endif //SPATACS_SYSTEMS_H
