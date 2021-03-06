//
// Created by erik on 11/20/16.
//

#ifndef SPATACS_CORE_ENTITYSTORAGE_H
#define SPATACS_CORE_ENTITYSTORAGE_H

#include <unordered_map>
#include <boost/range/irange.hpp>
#include <boost/range/adaptor/filtered.hpp>
#include <boost/range/adaptor/map.hpp>
#include <boost/range/algorithm/for_each.hpp>
#include "MetaStorage.h"
#include "ComponentStorage.h"
#include "core/meta/find.h"

namespace spatacs
{
namespace core
{
namespace ecs
{
    /*! \brief This class manages the storage of all entity data.
     *  \details It builts on ComponentStorage and MetadataStorage
     *      and ensure synchronicity between those. It is also responsible
     *      to translate Entity IDs into the subscript indices used to
     *      access the actual data.
     */
    template<class Config>
    class EntityStorage
    {
        using config_t       = Config;
        using cmp_storage_t  = typename Config::cmp_storage_t;
        using id_t           = typename Config::id_t;
        using meta_storage_t = MetadataStorage<MetaConfig<Config::comp_count + Config::tags_count, id_t, std::size_t>>;
        using bits_t         = typename meta_storage_t::bits_t;

        static_assert(type_v<id_t> != type_v<std::size_t>, "ID type and subscript type are indistinguishable!");

        template<class T>
        static constexpr std::size_t component_id = find(decay(type_v<T>), typename Config::comp_vec{});

    public:
        // --------------------------------------------------------------------------------
        //                      size related functions
        // --------------------------------------------------------------------------------
        //! resizes the buffers to accomodate \p new_size entities.
        EntityStorage& resize(std::size_t new_size);

        //! gets the current size of the buffers.
        std::size_t size() const noexcept;

        // --------------------------------------------------------------------------------
        //                              lifetime related functions
        // --------------------------------------------------------------------------------
        //! kills the object with \p id. This marks the object for deletion,
        //! and it will be removed during the next cleanup run.
        //! \note If the supplied \p id is invalid, nothing happens.
        EntityStorage& kill(id_t id) noexcept;

        //! Creates a new object, and returns the associated id.
        id_t create();

        //! Checks whether a given \p id corresponds to a living entity.
        //! Returns false if the id is not valid.
        bool is_alive(id_t id) const noexcept;

        // --------------------------------------------------------------------------------
        //                          direct access functions
        // --------------------------------------------------------------------------------
        //! gets the bits associated with \p id.
        //! \throw If \p id is not valid.
        const bits_t& bits(std::size_t index) const;

        //! gets the components associated with given subscript \p id.
        //! \throw If \p id is not valid.
        auto components(std::size_t id) const;

        //! gets the components associated with given subscript \p id (non const).
        //! \throw If \p id is not valid.
        auto mutable_components(std::size_t id);

        //! gets whether a component is alive, based directly on the id
        bool is_alive(std::size_t id) const;

        // --------------------------------------------------------------------------------
        //                          components functions
        // --------------------------------------------------------------------------------
        //! Adds a component to a given \p entity.
        //! \todo What do we do if that component already exists?
        template<class T, class... Args>
        T& add_component(id_t entity, type_t<T> component, Args... args);

        //! Removes a component from a given \p entity.
        template<class T>
        void remove_component(id_t entity, type_t<T> component);

        //! Checks for the existence of a component.
        template<class T>
        bool has_component(id_t entity, type_t<T> component);

        //! Gets the component (const) of a given \p entity.
        //! \note this does not check if the component is actually valid!
        template<class T>
        const T& get_component(id_t entity, type_t<T> component) const;

        //! Gets the component (const) of a given \p entity.
        //! \note this does not check if the component is actually valid!
        template<class T>
        T& get_mutable_component(id_t entity, type_t<T> component);

        // --------------------------------------------------------------------------------
        //                          tags functions
        // --------------------------------------------------------------------------------

        //! Adds a tag to an entity. Does nothing if the tag is already present.
        template<class T>
        void add_tag(id_t entity, type_t<T> tag);

        //! Removes a tag from a given \p entity. Does nothing if that tag was not set.
        template<class T>
        void remove_tag(id_t entity, type_t<T> tag);

        //! Checks for the existence of a tag in \p entity.
        template<class T>
        bool has_tag(id_t entity, type_t<T> tag);

        // --------------------------------------------------------------------------------
        //                          index functions
        // --------------------------------------------------------------------------------

        //! Gets the bitfield index of a component.
        template<class T>
        constexpr static std::size_t bit_index(type_t<T>);

        //! Gets the component tuple index of a component.
        template<class T>
        constexpr static std::size_t tuple_index(type_t<T>);

        //! Gets the bitfield index of a tag.
        template<class T>
        constexpr static std::size_t tag_index(type_t<T>);

        template<class... T>
        static bits_t get_bits(pack_t<T...>);

        /// looks up \p id and returns the corresponding subscript.
        std::size_t lookup(id_t id) const;

        /// gets the \p id that corresponds to a \p subscript
        id_t id_of(std::size_t subscript) const;

        // --------------------------------------------------------------------------------
        //                          ranges
        // --------------------------------------------------------------------------------
        //! Returns a range of all valid indices.
        //! \note The corresponging entity may be dead!
        auto index_range() const;

        //! Returns a range of all valid IDs. Valid IDs are IDs that correspond to an entity.
        //! \note The corresponging entity may be dead!
        auto id_range() const;
    private:
        // --------------------------------------------------------------------------------
        //                       helper functions
        // --------------------------------------------------------------------------------

        /// returns the next free subscript, resizing storage if necessary.
        std::size_t next_free();

        // --------------------------------------------------------------------------------
        //                      data storage
        // --------------------------------------------------------------------------------
        meta_storage_t mMetaData;
        cmp_storage_t  mComponents;

        // --------------------------------------------------------------------------------
        //                      Lookup table
        // --------------------------------------------------------------------------------
        std::unordered_map<id_t, std::size_t> mLookup;
        id_t mLastGivenID{};
    };

    // -----------------------------------------------------------------------------------------------------------------
    //                                      size functions
    // -----------------------------------------------------------------------------------------------------------------
    template<class C>
    EntityStorage<C>& EntityStorage<C>::resize(std::size_t new_size)
    {
        mMetaData.resize(new_size);
        mComponents.resize(new_size);

        return *this;
    }

    template<class C>
    std::size_t EntityStorage<C>::size() const noexcept
    {
        return mMetaData.size();
    }

    // -----------------------------------------------------------------------------------------------------------------
    //                              lifetime related functions
    // -----------------------------------------------------------------------------------------------------------------
    template<class C>
    EntityStorage<C>& EntityStorage<C>::kill(id_t id) noexcept
    {
        auto found = mLookup.find(id);
        // non-existent entities are always not alive
        if(found == mLookup.end())
            return *this;

        mMetaData.kill(found->second);
        return *this;
    }

    template<class C>
    auto EntityStorage<C>::create() -> id_t
    {
        std::size_t idx = next_free();

        mLookup[++mLastGivenID] = idx;
        mMetaData.create(idx, mLastGivenID);
        return mLastGivenID;
    }

    template<class C>
    bool EntityStorage<C>::is_alive(id_t id) const noexcept
    {
        auto found = mLookup.find(id);
        // non-existent entities are always not alive
        if(found == mLookup.end())
            return false;

        return is_alive(found->second);
    }

    // -----------------------------------------------------------------------------------------------------------------
    //                                          direct access functions
    // -----------------------------------------------------------------------------------------------------------------
    template<class C>
    auto EntityStorage<C>::bits(std::size_t index) const -> const bits_t&
    {
        return mMetaData.bits( index );
    }

    template<class C>
    auto EntityStorage<C>::components(std::size_t index) const
    {
        return mComponents.get( index );
    }
    template<class C>
    auto EntityStorage<C>::mutable_components(std::size_t index)
    {
        return mComponents.get( index );
    }

    template<class C>
    bool EntityStorage<C>::is_alive(std::size_t id) const
    {
        return mMetaData.is_alive(id);
    }

    // -----------------------------------------------------------------------------------------------------------------
    //                                      index functions
    // -----------------------------------------------------------------------------------------------------------------
    template<class C>
    template<class T>
    constexpr std::size_t EntityStorage<C>::bit_index(type_t<T>)
    {
        return component_id<T>;
    }

    template<class C>
    template<class T>
    constexpr std::size_t EntityStorage<C>::tag_index(type_t<T>)
    {
        return find(decay(type_v<T>), typename config_t::tags_vec{});
    }

    template<class C>
    template<class T>
    constexpr std::size_t EntityStorage<C>::tuple_index(type_t<T>)
    {
        return component_id<T>;
    }

    template<class C>
    std::size_t EntityStorage<C>::lookup(id_t id) const
    {
        return mLookup.at(id);
    }

    template<class C>
    auto EntityStorage<C>::id_of(std::size_t subscript) const -> id_t
    {
        return mMetaData.id_of(subscript);
    }

    template<class C>
    template<class... T>
    auto EntityStorage<C>::get_bits(pack_t<T...>) -> bits_t
    {
        /// \todo up to sizeof(ulong), we could to this constexpr.
        bits_t bits;
        auto A = {(bits.set(bit_index(type_t<T>{})), 0)...};
        (void)A;
        return bits;
    }

    // -----------------------------------------------------------------------------------------------------------------
    //                                      components functions
    // -----------------------------------------------------------------------------------------------------------------
    template<class C>
    template<class T>
    const T& EntityStorage<C>::get_component(id_t entity, type_t<T> component) const
    {
        return std::get<tuple_index(component)>( components(lookup(entity)) );
    }

    template<class C>
    template<class T>
    T& EntityStorage<C>::get_mutable_component(id_t entity, type_t<T> component)
    {
        return std::get<tuple_index(component)>( mutable_components(lookup(entity)) );
    }

    template<class C>
    template<class T>
    void EntityStorage<C>::remove_component(id_t entity, type_t<T> component)
    {
        mMetaData.mutable_bits(lookup(entity)).reset(bit_index(component));
    }

    template<class C>
    template<class T>
    bool EntityStorage<C>::has_component(id_t entity, type_t<T> component)
    {
        return bits(lookup(entity)).test(bit_index(component));
    }

    template<class C>
    template<class T, class... Args>
    T& EntityStorage<C>::add_component(id_t entity, type_t<T> component, Args... args)
    {
        // preparation
        auto index = lookup(entity);

        // update metadata
        mMetaData.mutable_bits(index).set(bit_index(component));

        // create new component, forward args to constructor.
        auto components = mComponents.get(index);
        std::get<tuple_index(component)>(components) = T(std::forward<Args>(args)...);

        return std::get<tuple_index(component)>(components);
    };

    // -----------------------------------------------------------------------------------------------------------------
    //                                      tags functions
    // -----------------------------------------------------------------------------------------------------------------
    template<class C>
    template<class T>
    void EntityStorage<C>::remove_tag(id_t entity, type_t<T> tag)
    {
        mMetaData.mutable_bits(lookup(entity)).reset(tag_index(tag));
    }

    template<class C>
    template<class T>
    bool EntityStorage<C>::has_tag(id_t entity, type_t<T> tag)
    {
        return bits(lookup(entity)).test(tag_index(tag));
    }

    template<class C>
    template<class T>
    void EntityStorage<C>::add_tag(id_t entity, type_t<T> tag)
    {
        mMetaData.mutable_bits(lookup(entity)).set(tag_index(tag));
    };

    // -----------------------------------------------------------------------------------------------------------------
    //                                              ranges
    // -----------------------------------------------------------------------------------------------------------------
    template<class C>
    auto EntityStorage<C>::index_range() const
    {
        return boost::irange(std::size_t(0), size());
    }

    template<class C>
    auto EntityStorage<C>::id_range() const
    {
        using namespace boost::adaptors;

        return mLookup | map_keys;
    }

    // -----------------------------------------------------------------------------------------------------------------
    //                                              helpers
    // -----------------------------------------------------------------------------------------------------------------
    template<class C>
    std::size_t EntityStorage<C>::next_free()
    {
        std::size_t idx = mMetaData.getNextFreeIndex();
        if(idx > size())
        {
            /// \todo growth strategy
            mMetaData.resize( size() + 1 );
            mComponents.resize( size() + 1);
            return mMetaData.getNextFreeIndex();
        }
        return idx;
    }
}
}
}

#endif //SPATACS_CORE_ENTITYSTORAGE_H
