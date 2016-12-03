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
    template<std::size_t bits_count>
    class MetadataStorage
    {
    public:
        using bits_t   = std::bitset<bits_count>;
        using bits_vec = std::vector<bits_t>;

        // size
        //! resizes all buffers
        void resize(std::size_t new_size);
        //! gets number of elements in all buffers.
        std::size_t size() const noexcept;

        // lifetime
        ///! sets an index to dead
        void kill(std::size_t index);

        //! sets an index to alive, and ensures that all bits are set to zero.
        void create(std::size_t index);

        //! checks whether a given index is alive
        bool is_alive(std::size_t index) const;

        // free id
        //! returns the next free ID. If none is found, returns
        //! numeric_limits<size_t>::max()
        std::size_t getNextFreeIndex() const noexcept;

        // bits
        //! get a constant reference to the bits at \p index.
        const bits_t& bits(std::size_t index) const;

        //! gets the bits as a mutable reference
        bits_t& mutable_bits(std::size_t index);

    private:
        bits_vec mComponentBits;
        //! This vector contains whether a specific entity is alive.
        std::vector<bool> mIsAlive;
    };

    // -------------------------------------------------------------------------------

    // lifetime
    template<std::size_t C>
    void MetadataStorage<C>::kill(std::size_t index)
    {
        mIsAlive.at(index) = false;
    }

    template<std::size_t C>
    void MetadataStorage<C>::create(std::size_t index)
    {
        mIsAlive.at(index) = true;
        mComponentBits.at(index).reset();
    }

    template<std::size_t C>
    bool MetadataStorage<C>::is_alive(std::size_t index) const
    {
        return mIsAlive.at(index);
    }

    template<std::size_t C>
    std::size_t MetadataStorage<C>::getNextFreeIndex() const noexcept
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

    template<std::size_t C>
    void MetadataStorage<C>::resize(std::size_t new_size)
    {
        mIsAlive.resize(new_size, false);
        mComponentBits.resize(new_size, 0);
    }

    template<std::size_t C>
    std::size_t MetadataStorage<C>::size() const noexcept
    {
        return mIsAlive.size();
    }

    // bits

    template<std::size_t C>
    auto MetadataStorage<C>::bits(std::size_t index) const -> const bits_t&
    {
        return mComponentBits.at(index);
    }

    template<std::size_t C>
    auto MetadataStorage<C>::mutable_bits(std::size_t index) -> bits_t&
    {
        return mComponentBits.at(index);;
    }
}
}
}

#endif //SPATACS_CORE_METASTORAGE_H
