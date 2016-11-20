//
// Created by erik on 11/20/16.
//

#ifndef SPATACS_CORE_ENTITYSTORAGE_H
#define SPATACS_CORE_ENTITYSTORAGE_H

#include <unordered_map>
#include "MetaStorage.h"
#include "ComponentStorage.h"

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
        using meta_storage_t = MetadataStorage<Config::comp_count>;
        using cmp_storage_t  = typename Config::cmp_storage_t;
        using id_t           = typename Config::id_t;
        using bits_t         = typename meta_storage_t::bits_t;
    public:
        // size related functions
        //! resizes the buffers to accomodate \p new_size entities.
        EntityStorage& resize(std::size_t new_size);

        //! gets the current size of the buffers.
        std::size_t size() const noexcept;

        // lifetime related functions
        //! kills the object with \p id. This marks the object for deletion,
        //! and it will be removed during the next cleanup run.
        //! \note If the supplied \p id is invalid, nothing happens.
        EntityStorage& kill(id_t id) noexcept;

        //! Creates a new object, and returns the associated id.
        id_t create();

        //! Checks whether a given \p id corresponds to a living entity.
        //! Returns false if the id is not valid.
        bool is_alive(id_t id) const noexcept;

        // bits functions
        //! gets the bits associated with \p id.
        //! \throw If \p id is not valid.
        const bits_t& bits(id_t id) const;

        //! gets the bits associated with \p id (non const).
        //! \throw If \p id is not valid.
        bits_t& mutable_bits(id_t id);

        // components functions
        //! gets the components associated with \p id.
        //! \throw If \p id is not valid.
        auto components(id_t id) const;

        //! gets the components associated with \p id (non const).
        //! \throw If \p id is not valid.
        auto mutable_components(id_t id);

    private:
        // helper functions
        /// looks up \p id and returns the corresponding subscript.
        std::size_t lookup(id_t id) const;

        /// returns the next free subscript, resizing storage if necessary.
        std::size_t next_free();

        // data storage
        meta_storage_t mMetaData;
        cmp_storage_t  mComponents;

        // Lookup table
        std::unordered_map<id_t, std::size_t> mLookup;
        id_t mLastGivenID{};
    };

    // size functions
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

    // lifetime related functions
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
        mMetaData.create(idx);
        return mLastGivenID;
    }

    template<class C>
    bool EntityStorage<C>::is_alive(id_t id) const noexcept
    {
        auto found = mLookup.find(id);
        // non-existent entities are always not alive
        if(found == mLookup.end())
            return false;

        return mMetaData.is_alive(found->second);
    }

    // bits functions
    template<class C>
    auto EntityStorage<C>::bits(id_t id) const -> const bits_t&
    {
        return mMetaData.bits( lookup(id) );
    }

    template<class C>
    auto EntityStorage<C>::mutable_bits(id_t id) -> bits_t&
    {
        return mMetaData.mutable_bits( lookup(id) );
    }

    // components functions

    template<class C>
    auto EntityStorage<C>::components(id_t id) const
    {
        return mComponents.get( lookup(id) );
    }
    template<class C>
    auto EntityStorage<C>::mutable_components(id_t id)
    {
        return mComponents.get( lookup(id) );
    }

    // helpers
    template<class C>
    std::size_t EntityStorage<C>::lookup(id_t id) const
    {
        return mLookup.at(id);
    }

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
