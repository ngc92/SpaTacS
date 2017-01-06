//
// Created by erik on 1/3/17.
//

#ifndef SPATACS_GAME_ENTITY_SUB_FWD_H
#define SPATACS_GAME_ENTITY_SUB_FWD_H

#include <cstdint>
#include "id.h"
#include "physics/units.h"
#include "core/ecs/fwd.h"
#include "../fwd.h"

namespace spatacs
{
namespace game
{
    namespace components
    {
        class EngineData;
        class FuelStorage;
        class PowerPlantData;
        class ShieldGeneratorData;
        constexpr auto sub_components = core::pack_v<EngineData, FuelStorage, PowerPlantData, ShieldGeneratorData>;
    }

    using SubEntityID = TaggedID<std::uint64_t, struct SubEntityTag>;

    // managers
    class FuelDistributor;

    namespace detail
    {
        using namespace components;
        constexpr auto SubEntitySetup = core::ecs::make_setup(core::type_v<SubEntityID>,
                                                               concat(components::common_components, components::sub_components),
                                                               core::pack_v<>);
        using SubEntityConfig = decltype(make_config(SubEntitySetup));
    }

    using SubEntityManager = core::EntityManager<detail::SubEntityConfig>;
    using SubEntityHandle  = core::EntityHandle<detail::SubEntityConfig>;

    namespace systems
    {
        using core::ecs::System;
        namespace signatures
        {
            using namespace components;
            using core::ecs::Signature;

            using Engine     = Signature<EngineData, const Health, const Activity>;
            using PowerPlant = Signature<const PowerPlantData, const Health, const Activity>;
            using FuelTank   = Signature<FuelStorage>;
        }
    }
}
}

#endif //SPATACS_GAME_ENTITY_SUB_FWD_H
