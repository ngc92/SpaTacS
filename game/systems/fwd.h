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
}
}

#endif //SPATACS_GAME_FWD_H
