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
#include "detail.h"

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
            //! Apply \p system to all components, allowing them to be changed.
            template<class System, class... Args>
            void apply(System&& system, Args&&... additional_args);

            //! Apply \p system to all components in a non-mutating way.
            template<class System, class... Args>
            void apply(System&& system, Args&&... additional_args) const;

            //! Apply \p system to a single entity \p target, allowing for mutation.
            template<class System, class... Args>
            auto apply_to(id_t target, System&& system, Args&&... additional_args);

            //! Apply \p system to a single entity \p target, not allowing for mutation.
            template<class System, class... Args>
            auto apply_to(id_t target, System&& system, Args&&... additional_args) const;

            //! \todo do we really need this function in the long run?
            template<class Signature>
            auto get_matching_ids(type_t<Signature>) const;
        private:

            template<class System, class F, class IDRange, class... Args>
            void apply_imp(System&& system, IDRange&& id_range, F&& get_components, Args&&... additional_args) const;

            // Storage
            storage_t mStorage;
        };

        /*! \brief Implementation of the apply function.
         * @tparam C \p Config parameter of the EntitiyManager
         * @tparam System Type of the System that is passed. Needs an accessible typedef \p signarture_t
         *                  that specifies which components should be passed to its operator().
         * @param system The system that will be applied to all entities (that match its signature).
         * @param get_components A function that, for a given storage index, returns the components of
         *                      the corresponding entity. Depending on whether the application is
         *                      to a const EntityManager or not, this getter function can either retrieve
         *                      mutable components or const components. The dispatch is handled through the
         *                      apply() oververloads.
         * @param
         *
         */
        template<class C>
        template<class System, class F, class IDRange, class... Args>
        void EntityManager<C>::apply_imp(System&& system, IDRange&& id_range, F&& get_components, Args&&... additional_args) const
        {
            static_assert(detail::has_signature_v<System>, "Could not find typedef \"signature_t\" in \"system\"");

            // extract relevant types
            using namespace detail;

            using system_t = std::decay_t<System>;
            using signature_t = typename system_t::signature_t;
            constexpr typename signature_t::types_t types{};

            // curry System by applying additional args
            auto partially_applied = [&](auto&&... rest)
            {
                return system(std::forward<decltype(rest)>(rest)..., std::forward<Args>(additional_args)...);
            };

            // create a functor that forwards the correct components.
            auto system_functor = make_forwarder( partially_applied, type_v<C>, types, get_components);

            // get the sytem bits
            auto system_bits = mStorage.get_bits(types);

            // functor that checks that bits match
            auto matcher = [&](std::size_t index) {
                return mStorage.is_alive(index) && match_bits(system_bits, mStorage.bits(index));
            };

            for_each(id_range | boost::adaptors::filtered(matcher), system_functor);
        }

        // specific apply functions in const and non-const variations. These just call
        // apply_imp with correctly specified getter.
        template<class C>
        template<class System, class... Args>
        void EntityManager<C>::apply(System&& system, Args&&... additional_args)
        {
            auto getter = [&](std::size_t index)
            {
                return mStorage.mutable_components(index);
            };

            apply_imp(std::forward<System>(system), mStorage.index_range(), getter, std::forward<Args>(additional_args)...);
        }

        template<class C>
        template<class System, class... Args>
        void EntityManager<C>::apply(System&& system, Args&&... additional_args) const
        {
            auto getter = [&](std::size_t index)
            {
                return mStorage.components(index);
            };

            apply_imp(std::forward<System>(system), mStorage.index_range(), getter, std::forward<Args>(additional_args)...);
        }

        template<class C>
        template<class System, class... Args>
        auto EntityManager<C>::apply_to(id_t target, System&& system, Args&&... additional_args) const
        {
            auto getter = [&](std::size_t index)
            {
                return mStorage.components(index);
            };

            std::array<std::size_t, 1> ids = {mStorage.lookup(target)};

            apply_imp(std::forward<System>(system), ids, getter, std::forward<Args>(additional_args)...);
        }

        template<class C>
        template<class System, class... Args>
        auto EntityManager<C>::apply_to(id_t target, System&& system, Args&&... additional_args)
        {
            auto getter = [&](std::size_t index)
            {
                return mStorage.mutable_components(index);
            };

            std::array<std::size_t, 1> ids = {mStorage.lookup(target)};

            apply_imp(std::forward<System>(system), ids, getter, std::forward<Args>(additional_args)...);
        }

        template<class C>
        template<class Signature>
        auto EntityManager<C>::get_matching_ids(type_t<Signature>) const
        {
            constexpr typename Signature::types_t types{};
            // get the sytem bits
            auto system_bits = mStorage.get_bits(types);

            // functor that checks that bits match
            auto matcher = [&](std::size_t index) {
                return detail::match_bits(system_bits, mStorage.bits(index));
            };

            return mStorage.index_range() |
                    boost::adaptors::filtered(matcher) |
                    boost::adaptors::transformed([&](std::size_t index){
                        return mStorage.id_of(index);
                    });
        }
    }
}
}

#endif //SPATACS_CORE_ENTITYMANAGER_H
