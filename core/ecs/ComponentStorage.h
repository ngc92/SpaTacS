//
// Created by erik on 11/20/16.
//

#ifndef SPATACS_CORE_COMPONENTSTORAGE_H
#define SPATACS_CORE_COMPONENTSTORAGE_H

#include <tuple>
#include <utility>
#include <vector>

namespace spatacs {
namespace core {
namespace ecs {

    /*! \brief Class that stores the (POD) components
     *  \details This class manages owns the components
     *      and manages accessing them. Access is provided
     *      through get methods, that return tuples of
     *      (const) references. This is pruposefully defined
     *      as a pretty dumb data structure. It is mostly a
     *      container that manages different data in a
     *      "Struct of Arrays" manner.
     *      \todo maybe rename stuff so that is does not
     *          sound specific.
     */
    template<class... Components>
    class ComponentStorage
    {
        //! number of component types
        static constexpr const std::size_t cmp_count = sizeof...(Components);
        //! an integer sequence that gives all the component indices.
        using index_list = std::make_index_sequence<cmp_count>;
        static_assert( cmp_count >= 1, "ComponentStorage needs at least one component type." );


        //! tuple of vectors that store the components (struct of arrays)
        using cvec_type  = std::tuple<std::vector <Components>...>;

        //! tuple type that contains references to all components.
        using ref_tpl_t  = std::tuple<Components& ...>;
        //! tuple type that contains const references to all components.
        using cref_tpl_t = std::tuple<const Components& ...>;

    public:
        // size related functions
        //! resizes the buffers for every component to \p new_size elements.
        void resize(std::size_t new_size)
        {
            // resize all component arrays
            resize_all(new_size, index_list{});
        }

        //! gets the current size of the buffers.
        std::size_t size() const noexcept
        {
            return std::get<0>(mData).size();
        }

        // getters
        //! gets the components at index \p index.
        ref_tpl_t get(std::size_t index)
        {
            return get(index, index_list{});
        }

        //! gets the components (const) at \p index.
        cref_tpl_t get(std::size_t index) const
        {
            return get(index, index_list{});
        }
    private:

        template<std::size_t... idx>
        void resize_all(std::size_t new_size, std::index_sequence<idx...>)
        {
            std::initializer_list<int> A{(std::get<idx>(mData).resize(new_size), 0)...};
            (void)A; // fix unused warning
        }

        template<std::size_t... idx>
        ref_tpl_t get(std::size_t index, std::index_sequence<idx...>)
        {
            return std::tie(std::get<idx>(mData).at(index)...);
        }

        template<std::size_t... idx>
        cref_tpl_t get(std::size_t index, std::index_sequence<idx...>) const
        {
            return std::tie(std::get<idx>(mData).at(index)...);
        }

        //! This contains all the component data.
        cvec_type mData;
    };
}
}
}


#endif //SPATACS_CORE_COMPONENTSTORAGE_H
