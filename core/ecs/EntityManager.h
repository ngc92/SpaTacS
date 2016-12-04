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
            template<class System>
            void apply(System&& system);

            //! Apply \p system to all components in a non-mutating way.
            template<class System>
            void apply(System&& system) const;

            //! Apply \p system to a single component, allowing for mutation.
            template<class System>
            auto apply_to(System&& system, id_t target);

            //! Apply \p system to a single component, not allowing for mutation.
            template<class System>
            auto apply_to(System&& system, id_t target) const;

            //! \todo do we really need this function in the long run?
            template<class Signature>
            auto get_matching_ids(type_t<Signature>) const;
        private:

            template<class System, class F, class IDRange>
            void apply_imp(System&& system, IDRange&& id_range, F&& get_components) const;

            // Storage
            storage_t mStorage;
        };

        namespace detail {
            template<class Bitfield>
            bool match_bits(const Bitfield& required, const Bitfield& available)
            {
                return (required & available) == required;
            }

            /// get from tuple
            template<class T, class Comps>
            auto& get_component(Comps&& comps, type_t<T> type)
            {
                constexpr std::size_t id = find(decay(type), decay(args_of(type_v<Comps>)));
                return std::get<id>(comps);
            }


            template<class Index, class T, class CG>
            auto& get_argument_dispatch(Index&& index, type_t<T> type, CG&& get_components)
            {
                return get_component(get_components(std::forward<Index>(index)), type);
            };

            template<class Index, class Config, class T, class CG>
            auto& get_single_argument(Index&& index, type_t<Config> config, type_t<T> type, CG&& cg)
            {
                return get_argument_dispatch(std::forward<Index>(index),
                                             type,
                                             std::forward<CG>(cg));
            }

            /*! \brief Creates a forwarder function for System arguments
             *  \details This is a higher-order function that returns a lambda
             *           that takes an entity index as an input and applies
             *           \p system_f to the  requested (according to \p signature)
             *           components.
             *           The components (and other possible function parameters) are
             *           retrieved by calls to the corresponding getter functions. These calls
             *           only happen when the Signature requests the corresponding info.
             *           This function itself does nothing more than expanding the call for
             *           all \p Types to be passed to get_single_argument.
             *
             * @param system         Forwarding reference to the system.
             * @param argument_types Parameter pack that contains the information about the System signature. Used to
             *                       deduce that \p Types template arguments.
             * @param config         Transmitts the \p Config of the EntityManager.
             * @param get_components A function that, given an index, returns the components that belong to that given
             *                       index as a tuple of references.
             * @return A function that has to be applied to a components tuple.
             */
            template<class System, class Config, class ComponentGetter, class... Types>
            auto make_forwarder(System&& system, type_t<Config> config, pack_t<Types...> argument_types,
                                ComponentGetter&& get_components)
            {
                // components gets passed a tuple with all the components of the entity.
                return [&](auto&& index) {
                    //! \todo maybe we can static assert that \p index has a sensible type.
                    return system( get_single_argument(index, config, type_v<Types>,
                                                       std::forward<ComponentGetter>(get_components))... );
                };
            };

            template<class T>
            struct has_signature_helper
            {
                using base_t = std::decay_t<T>;
                using yes    = std::uint8_t;
                using no     = std::uint16_t;

                template<class U>
                static yes test(int, typename U::signature_t* p = nullptr);

                template<class U>
                static no  test(...);

                constexpr static bool result = std::is_same<decltype(test<base_t>(1)), yes>::value;
            };

            template<class T>
            constexpr bool has_signature_v = detail::has_signature_helper<T>::result;
        }


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
         *
         */
        template<class C>
        template<class System, class F, class IDRange>
        void EntityManager<C>::apply_imp(System&& system, IDRange&& id_range, F&& get_components) const
        {
            static_assert(detail::has_signature_v<System>, "Could not find typedef \"signature_t\" in \"system\"");

            // extract relevant types
            using namespace detail;

            using system_t = std::decay_t<System>;
            using signature_t = typename system_t::signature_t;
            constexpr typename signature_t::types_t types{};

            // create a functor that forwards the correct components.
            auto system_functor = make_forwarder( std::forward<System>(system), type_v<C>, types, get_components);

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
        template<class System>
        void EntityManager<C>::apply(System&& system)
        {
            auto getter = [&](std::size_t index)
            {
                return mStorage.mutable_components(index);
            };

            apply_imp(std::forward<System>(system), mStorage.index_range(), getter);
        }

        template<class C>
        template<class System>
        void EntityManager<C>::apply(System&& system) const
        {
            auto getter = [&](std::size_t index)
            {
                return mStorage.components(index);
            };

            apply_imp(std::forward<System>(system), mStorage.index_range(), getter);
        }

        template<class C>
        template<class System>
        auto EntityManager<C>::apply_to(System&& system, id_t target) const
        {
            auto getter = [&](std::size_t index)
            {
                return mStorage.components(index);
            };

            std::array<std::size_t, 1> ids = {mStorage.lookup(target)};

            apply_imp(std::forward<System>(system), ids, getter);
        }

        template<class C>
        template<class System>
        auto EntityManager<C>::apply_to(System&& system, id_t target)
        {
            auto getter = [&](std::size_t index)
            {
                return mStorage.mutable_components(index);
            };

            std::array<std::size_t, 1> ids = {mStorage.lookup(target)};

            apply_imp(std::forward<System>(system), ids, getter);
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
