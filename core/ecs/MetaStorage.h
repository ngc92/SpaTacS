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
    /*! \brief This class stores the metadata associated to each component.
     *  \details This is a very low level class that manages vectors of bit fields
     *          that contain this information about which components are present,
     *          a vector that saves whether the component is alive.
     *  \todo Add entity versioning information here.
     */
    template<std::size_t bits_count, class ID>
    class MetadataStorage
    {
    public:
        using bits_t   = std::bitset<bits_count>;
        using bits_vec = std::vector<bits_t>;
        using id_t     = ID;

        // size
        //! resizes all buffers
        void resize(std::size_t new_size);
        //! gets number of elements in all buffers.
        std::size_t size() const noexcept;

        // lifetime
        ///! sets an index to dead
        void kill(std::size_t index);

        //! sets an index to alive, and ensures that all bits are set to zero.
        //! \param id the ID that will be associated with the new entity.
        void create(std::size_t index, id_t id);

        //! checks whether a given index is alive
        bool is_alive(std::size_t index) const;

        // ids
        //! returns the next free ID. If none is found, returns
        //! numeric_limits<size_t>::max()
        std::size_t getNextFreeIndex() const noexcept;

        //! gets the id that is associated with a subscript
        id_t id_of(std::size_t index) const;

        // bits
        //! get a constant reference to the bits at \p index.
        const bits_t& bits(std::size_t index) const;

        //! gets the bits as a mutable reference
        bits_t& mutable_bits(std::size_t index);

    private:
        bits_vec mComponentBits;
        //! This vector contains whether a specific entity is alive.
        std::vector<bool> mIsAlive;
        //! This vector contains the IDs of all the entities.
        std::vector<id_t> mIDs;
    };

    // -------------------------------------------------------------------------------

    // lifetime
    template<std::size_t C, class ID>
    void MetadataStorage<C, ID>::kill(std::size_t index)
    {
        mIsAlive.at(index) = false;
    }

    template<std::size_t C, class ID>
    void MetadataStorage<C, ID>::create(std::size_t index, id_t id)
    {
        mIsAlive.at(index) = true;
        mIDs.at(index)     = id;
        mComponentBits.at(index).reset();
    }

    template<std::size_t C, class ID>
    bool MetadataStorage<C, ID>::is_alive(std::size_t index) const
    {
        return mIsAlive.at(index);
    }

    template<std::size_t C, class ID>
    auto MetadataStorage<C, ID>::id_of(std::size_t index) const -> id_t
    {
        return mIDs.at(index);
    };

    template<std::size_t C, class ID>
    std::size_t MetadataStorage<C, ID>::getNextFreeIndex() const noexcept
    {
        /// \todo do not always start crawling for free indices
        ///       in the beginning.
        for(std::size_t i = 0; i < mIsAlive.size(); ++i)
        {
            if(mIsAlive[i] == false)
                return i;
        }

        return std::numeric_limits<std::size_t>::max();
    }

    // size

    template<std::size_t C, class ID>
    void MetadataStorage<C, ID>::resize(std::size_t new_size)
    {
        mIsAlive.resize(new_size, false);
        mIDs.resize(new_size);
        mComponentBits.resize(new_size, 0);
    }

    template<std::size_t C, class ID>
    std::size_t MetadataStorage<C, ID>::size() const noexcept
    {
        return mIsAlive.size();
    }

    // bits

    template<std::size_t C, class ID>
    auto MetadataStorage<C, ID>::bits(std::size_t index) const -> const bits_t&
    {
        return mComponentBits.at(index);
    }

    template<std::size_t C, class ID>
    auto MetadataStorage<C, ID>::mutable_bits(std::size_t index) -> bits_t&
    {
        return mComponentBits.at(index);;
    }
}
}
}

#endif //SPATACS_CORE_METASTORAGE_H
