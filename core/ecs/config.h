//
// Created by erik on 1/4/17.
//

#ifndef SPATACS_CORE_ECS_CONFIG_H
#define SPATACS_CORE_ECS_CONFIG_H

#include <cstdint>
#include "core/meta/types.h"
#include "core/ecs/fwd.h"

namespace spatacs
{
namespace core
{
namespace ecs
{
    namespace config
    {
        template<class ID, class Comps, class Tags>
        struct Setup
        {
            constexpr Setup() = default;
            using id_t         = ID;
            using components_t = Comps;
            using tags_t       = Tags;
        };

        template<class S>
        struct Config
        {
            constexpr Config() = default;
            // types
            using this_t        = Config<S>;
            using comp_vec      = typename S::components_t;
            using tags_vec      = typename S::tags_t;
            using id_t          = typename S::id_t;

            constexpr static auto cmp_storage_v = substitute(template_v<ComponentStorage>, comp_vec{});

            using cmp_storage_t = typename decltype(cmp_storage_v)::type;
            using storage_t     = EntityStorage<this_t>;
            using handle_t      = EntityHandle<this_t>;
            using manager_t     = EntityManager<this_t>;

            // constant
            static const constexpr std::size_t comp_count = comp_vec::size;
            static const constexpr std::size_t tags_count = tags_vec::size;
        };

        template<class S>
        constexpr auto make_config(S s)
        {
            return Config<S>();
        }

        template<class ID, class...P, class... T>
        constexpr auto make_setup(type_t<ID>, pack_t<P...>, pack_t<T...>)
        {
            return Setup<ID, pack_t<P...>, pack_t<T...>>();
        };
    }

    using config::Setup;
    using config::make_setup;
}
}
}

#endif //SPATACS_CORE_ECS_CONFIG_H
