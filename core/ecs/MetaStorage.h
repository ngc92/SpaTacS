//
// Created by erik on 11/20/16.
//

#ifndef SPATACS_CORE_METASTORAGE_H
#define SPATACS_CORE_METASTORAGE_H

#include <bitset>
#include <vector>
#include <limits>

namespace spatacs
{
namespace core
{
namespace ecs
{
    template<std::size_t Bits, class ID, class Subscript>
    struct MetaConfig
    {
        constexpr static std::size_t bits_count = Bits;
        using id_t        = ID;
        using subscript_t = Subscript;
    };

    /*! \brief This class stores the metadata associated to each component.
     *  \details This is a very low level class that manages vectors of bit fields
     *          that contain this information about which components are present,
     *          a vector that saves whether the component is alive.
     *  \todo Add entity versioning information here.
     */
    template<class Config>
    class MetadataStorage
    {
    public:
        constexpr static std::size_t bits_count = Config::bits_count;
        using bits_t      = std::bitset<bits_count>;
        using bits_vec    = std::vector<bits_t>;
        using id_t        = typename Config::id_t;
        using subscript_t = typename Config::subscript_t;

        // size
        //! resizes all buffers
        void resize(std::size_t new_size);
        //! gets number of elements in all buffers.
        std::size_t size() const noexcept;

        // lifetime
        ///! sets an index to dead
        void kill(subscript_t index);

        //! sets an index to alive, and ensures that all bits are set to zero.
        //! \param id the ID that will be associated with the new entity.
        void create(subscript_t index, id_t id);

        //! checks whether a given index is alive
        bool is_alive(subscript_t index) const;

        // ids
        //! returns the next free ID. If none is found, returns
        //! numeric_limits<size_t>::max()
        subscript_t getNextFreeIndex() const noexcept;

        //! gets the id that is associated with a subscript
        id_t id_of(subscript_t index) const;

        // bits
        //! get a constant reference to the bits at \p index.
        const bits_t& bits(subscript_t index) const;

        //! gets the bits as a mutable reference
        bits_t& mutable_bits(subscript_t index);

    private:
        bits_vec mComponentBits;
        //! This vector contains whether a specific entity is alive.
        std::vector<bool> mIsAlive;
        //! This vector contains the IDs of all the entities.
        std::vector<id_t> mIDs;
    };

    // -------------------------------------------------------------------------------

    // lifetime
    template<class C>
    void MetadataStorage<C>::kill(subscript_t index)
    {
        mIsAlive.at(index) = false;
    }

    template<class C>
    void MetadataStorage<C>::create(subscript_t index, id_t id)
    {
        mIsAlive.at(index) = true;
        mIDs.at(index)     = id;
        mComponentBits.at(index).reset();
    }

    template<class C>
    bool MetadataStorage<C>::is_alive(subscript_t index) const
    {
        return mIsAlive.at(index);
    }

    template<class C>
    auto MetadataStorage<C>::id_of(subscript_t index) const -> id_t
    {
        return mIDs.at(index);
    };

    template<class C>
    auto MetadataStorage<C>::getNextFreeIndex() const noexcept -> subscript_t
    {
        /// \todo do not always start crawling for free indices
        ///       in the beginning.
        for(subscript_t i{0}; i < mIsAlive.size(); ++i)
        {
            if(mIsAlive[i] == false)
                return i;
        }

        return std::numeric_limits<std::size_t>::max();
    }

    // size

    template<class C>
    void MetadataStorage<C>::resize(std::size_t new_size)
    {
        mIsAlive.resize(new_size, false);
        mIDs.resize(new_size);
        mComponentBits.resize(new_size, 0);
    }

    template<class C>
    std::size_t MetadataStorage<C>::size() const noexcept
    {
        return mIsAlive.size();
    }

    // bits

    template<class C>
    auto MetadataStorage<C>::bits(subscript_t index) const -> const bits_t&
    {
        return mComponentBits.at(index);
    }

    template<class C>
    auto MetadataStorage<C>::mutable_bits(subscript_t index) -> bits_t&
    {
        return mComponentBits.at(index);;
    }
}
}
}

#endif //SPATACS_CORE_METASTORAGE_H
