//
// Created by erik on 12/3/16.
//

#ifndef SPATACS_GAME_FWD_H
#define SPATACS_GAME_FWD_H

#include "core/ecs/System.h"
#include "physics/units.h"
#include "core/ecs/fwd.h"
#include "id.h"

namespace spatacs
{
namespace game
{
    class Starship;
    class EnergyManager;

    class FuelStorage;
    class AmmoStorage;
    class EngineData;
    class Health;
    class Activity;
    class ShieldGeneratorData;
    class PowerPlantData;
    class LifeSupportData;
    class WeaponAimData;
    class ProjectileWpnData;
    class Timer;
    class Name;

    using CompEntID = TaggedID<std::uint64_t, struct CpmEntTag>;

    using SubsystemConfig = core::ecs::Config<CompEntID, Health, FuelStorage, EngineData,
            PowerPlantData, ShieldGeneratorData, LifeSupportData, WeaponAimData, ProjectileWpnData,
            Timer, Name, AmmoStorage, Activity>;

    using SubsystemManager = core::EntityManager<SubsystemConfig>;
    using SubsystemHandle  = core::EntityHandle<SubsystemConfig>;

    namespace systems {
        namespace signatures {
            using ListAmmoSignature    = core::ecs::Signature<const AmmoStorage>;
            using ChangeAmmoSignature  = core::ecs::Signature<AmmoStorage>;
            using LifeSupportSignature = core::ecs::Signature<LifeSupportData>;
            using PowerProductionSignature = core::ecs::Signature<const PowerPlantData, const Health, const Activity>;
            using PropulsionSignature  = core::ecs::Signature<EngineData, const Health, const Activity>;
            using ShieldSignature      = core::ecs::Signature<ShieldGeneratorData, const Health, const Activity>;
            using TankInfoSignature    = core::ecs::Signature<const FuelStorage>;
            using ModifyFuelSignature  = core::ecs::Signature<FuelStorage>;
            using CountdownSignature   = core::ecs::Signature<Timer>;
            using AimSignature         = core::ecs::Signature<const WeaponAimData, const Health>;
        }
    }
}
}

#endif //SPATACS_GAME_FWD_H
