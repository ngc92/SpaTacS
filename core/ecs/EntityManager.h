//
// Created by erik on 11/19/16.
//

#ifndef SPATACS_CORE_ENTITYMANAGER_H
#define SPATACS_CORE_ENTITYMANAGER_H

#include <cstdint>
#include <vector>
#include <tuple>
#include <unordered_map>
#include "core/meta/find.h"
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
            using comp_vec      = pack_t<Components...>;
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

            auto id_range() const { return mStorage.id_range(); }

            // System application
            template<class System>
            void apply(System&& system);

            template<class System>
            void apply(System&& system) const;
        private:

            template<class System, class F>
            void apply_imp(System&& system, F&& get_components) const;

            // Storage
            storage_t mStorage;
        };

        namespace detail {
            template<class Bitfield>
            bool match_bits(const Bitfield& required, const Bitfield& available)
            {
                return (required & available) == required;
            }

            /// \todo add tags that allow passing other data,
            ///       such as IDs, handles, user data, optional components etc.
            template<class T, class... C>
            auto& find_single(std::tuple<C...>& comps)
            {
                using search_t = type_t<std::decay_t<T>>;
                using haystack_t = pack_t<std::decay_t<C>...>;

                constexpr std::size_t id = find(search_t{}, haystack_t{});
                return std::get<id>(comps);
            }

            template<class Input, class... Types>
            auto make_forwarder(Input&& ip, pack_t<Types...>)
            {
                return [&](auto&& components) {
                    return ip(find_single<Types>(components)...);
                };
            };
        }


        /*! \brief Implementation of the apply function.
         * @tparam C \p Config parameter of the EntitiyManager
         * @tparam System Type of the System that is passed. Needs an accessible typedef \p signarture_t
         *                  that specifies which components should be passed to its operator().
         * @tparam F Function type of the \p get_components function.
         * @param system The system that will be applied to all entities (that match its signature).
         * @param get_components A function that, for a given storage index, returns the components of
         *                      the corresponding entity. Depending on whether the application is
         *                      to a const EntityManager or not, this getter function can either retrieve
         *                      mutable components or const components. The dispatch is handled through the
         *                      apply() oververloads.
         */
        template<class C>
        template<class System, class F>
        void EntityManager<C>::apply_imp(System&& system, F&& get_components) const
        {
            // extract relevant types
            using namespace detail;

            using system_t = std::decay_t<System>;
            using signature_t = typename system_t::signature_t;
            using types_t  = typename signature_t::types_t;

            // get the sytem bits
            auto system_bits = mStorage.get_bits(types_t{});

            // create a functor that forwards the correct components.
            auto apply_comps = make_forwarder( std::forward<System>(system), types_t{} );

            // functor that checks that bits match
            auto matcher = [&](std::size_t index) {
                return match_bits(system_bits, mStorage.bits(index));
            };

            auto f = [&](std::size_t index)
            {
                apply_comps(get_components(index));
            };

            for_each(mStorage.index_range() | boost::adaptors::filtered(matcher), f);
        }

        // specific apply functions in const and non-const variations. These just call
        // apply_imp with correctly specified getter.
        template<class C>
        template<class System>
        void EntityManager<C>::apply(System&& system)
        {
            auto getter = [&](std::size_t index)
            {
                return mStorage.mutable_components(index);
            };

            apply_imp(std::forward<System>(system), getter);
        }

        template<class C>
        template<class System>
        void EntityManager<C>::apply(System&& system) const
        {
            auto getter = [&](std::size_t index)
            {
                return mStorage.components(index);
            };

            apply_imp(std::forward<System>(system), getter);
        }
    }
}
}

#endif //SPATACS_CORE_ENTITYMANAGER_H
