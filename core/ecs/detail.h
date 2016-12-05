//
// Created by erik on 12/5/16.
//

#ifndef SPATACS_CORE_ECS_DETAIL_H
#define SPATACS_CORE_ECS_DETAIL_H

namespace spatacs
{
namespace core
{
namespace ecs
{
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

    //!  \todo there should be a nicer SFINAE trick
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

        constexpr static bool value = std::is_same<decltype(test<base_t>(1)), yes>::value;
    };

    template<class T>
    constexpr bool has_signature_v = has_signature_helper<T>::value;
}
}
}
}

#endif //SPATACS_CORE_ECS_DETAIL_H
