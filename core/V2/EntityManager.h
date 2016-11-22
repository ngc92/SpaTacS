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
#include "fwd.h"
#include "EntityStorage.h"

namespace spatacs
{
namespace core
{
    namespace ecs
    {
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

            //! This function creates a handle to an entity with id \p entity. It does not check
            //! whether \p entity is actually valid!
            handle_t get(id_t entity) { return {*this, entity}; }

            template<class T, class... Args>
            T& add_component(id_t entity, Args&&... args)
            {
                return mStorage.add_component(entity, type_t<T>{}, std::forward<Args>(args)...);
            };

            template<class T>
            void remove_component(id_t entity) { return mStorage.add_component(entity, type_t<T>{}); }

            template<class T>
            bool has_component(id_t entity) { return mStorage.has_component(entity, type_t<T>{}); }

            template<class T>
            const T& get_component(id_t entity) const { return mStorage.get_component(entity, type_t<T>{}); }

            template<class T>
            T& get_mutable_component(id_t entity) { return mStorage.get_mutable_component(entity, type_t<T>{}); }

            // System application
            template<class System>
            void apply(System&& system);
        private:
            // Storage
            storage_t mStorage;
        };

        template<class Bitfield>
        bool match_bits(const Bitfield& required, const Bitfield& available)
        {
            return (required & available) == required;
        }

        template<class C>
        template<class System>
        void EntityManager<C>::apply(System&& system)
        {
            using signature_t = typename System::signature_t;

            // get the sytem bits
            auto system_bits = mStorage.get_bits(typename signature_t::types_t{});

            // create a functor that forwards the correct components.
            auto apply_comps = signature_t::forwarder( std::forward<System>(system) );

            auto f = [&](std::size_t index)
            {
                if(match_bits(system_bits, mStorage.bits(index)))
                {
                    apply_comps(mStorage.mutable_components(index));
                }
            };
            mStorage.iterate_indices(f);
        }
    }
}
}

#endif //SPATACS_CORE_ENTITYMANAGER_H
