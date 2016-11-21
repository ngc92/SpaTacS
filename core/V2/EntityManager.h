//
// Created by erik on 11/19/16.
//

#ifndef SPATACS_CORE_ENTITYMANAGER_H
#define SPATACS_CORE_ENTITYMANAGER_H

#include <cstdint>
#include <vector>
#include <tuple>
#include <unordered_map>
#include "mp.h"

namespace spatacs
{
namespace core
{
    namespace ecs
    {
        // forward declarations
        template<class... Components>
        class ComponentStorage;

        template<class Config>
        class EntityHandle;

        template<class Config>
        class EntityStorage;

        template<class Signature>
        class System;

        template<class Config>
        class EntityManager;

        /*! \brief Type that collects all information to configure our ECS.
         *  \details Configuration includes:
         *      All possible components types.
         */
        template<class ID, class... Components>
        struct Config {
            // types
            using this_t        = Config<ID, Components...>;
            using comp_vec      = type_vec_t<Components...>;
            using id_t          = ID;
            using cmp_id_type   = TaggedID<std::uint64_t, this_t>;
            using cmp_storage_t = ComponentStorage<Components...>;

            using storage_t     = EntityStorage<this_t>;

            using handle_t      = EntityHandle<this_t>;
            using manager_t     = EntityManager<this_t>;

            // constant
            static const constexpr std::size_t comp_count = sizeof...(Components);
        };

        // -----------------------------------------------------------------------------------------------------

        template<class Config>
        class EntityManager
        {
        public:
            // get types
            using config_t  = Config;
            using id_t      = typename Config::id_t;
            using storage_t = typename Config::storage_t;
            using handle_t  = typename Config::handle_t;

            // ----------------------------------------------------------------
            //              Storage wrapper functions
            // ----------------------------------------------------------------
            EntityManager& kill(id_t entity) noexcept { mStorage.kill(entity); return *this; }
            bool is_alive(id_t entity) const noexcept { return mStorage.is_alive(entity); }
            handle_t create() { return {*this, mStorage.create()}; }

            template<class T, class... Args>
            T& add_component(id_t entity, Args&&... args)
            {
                return mStorage.add_component(entity, type_t<T>{}, std::forward<Args>(args)...);
            };

            template<class T>
            void remove_component(id_t entity) { return mStorage.add_component(entity, type_t<T>{}); }

            template<class T>
            bool has_component(id_t entity) { return mStorage.has_components(entity); }

            template<class T>
            const T& get_component(id_t entity) const { return mStorage.get_component(entity, type_t<T>{}); }

            template<class T>
            T& get_mutable_component(id_t entity) { return mStorage.get_mutable_component(entity, type_t<T>{}); }

            // System application
            template<class Signature>
            void apply(System<Signature>&& system);
        private:
            // Storage
            storage_t mStorage;
        };

        template<class Bitfield>
        bool match_bits(const Bitfield& required, const Bitfield& available)
        {
            return required & available == required;
        }

        template<class C>
        template<class S>
        void EntityManager<C>::apply(System<S>&& system)
        {
            typename C::bits_t system_bits;
            auto apply_comps = S::forwarder(std::forward<System<S>>(system));

            auto f = [&](std::size_t index)
            {
                if(match_bits(system_bits, mStorage.bits(index)))
                {
                    apply_comps(mStorage.components(index));
                }
            };
            mStorage.iterate_indices(f);
        }
    }
}
}

#endif //SPATACS_CORE_ENTITYMANAGER_H
