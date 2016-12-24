//
// Created by erik on 11/1/16.
//

#ifndef SPATACS_ID_H
#define SPATACS_ID_H

#include <iosfwd>

namespace spatacs
{
    /*! \brief A class for IDs, that provides an additional template parameter as a tag.
     *  \details This allows to define several distinct IDs that cannot be intermixed.
     *  IDs are expected to be of integral type, but do not provide most of the usual integer
     *  operators. Supported are:
     *    stream output
     *    equality comparison
     *    less than comparison (so they can be used in ordered containers)
     *    std::hash (so they can be used in unordered containers).
     *  A value of 0 indicates a non-assigned ID.
     *  Most operations are declared noexcept constexpr, so this class could even be used for compile time IDs.
     */
    template<class Int, class Tag>
    class TaggedID
    {
    public:
        constexpr TaggedID() noexcept = default;
        constexpr explicit TaggedID(Int i) noexcept : mValue(i) {};

        constexpr Int getID() const noexcept { return mValue; }
        constexpr explicit operator bool() const noexcept { return mValue != 0; }
        constexpr explicit operator Int() const noexcept  { return mValue; }
        constexpr void reset() noexcept { mValue = 0; }

        constexpr TaggedID& operator++() noexcept { ++mValue; return *this; }

    private:
        Int mValue{0};
    };

    template<class O, class I, class T>
    std::basic_ostream<O>& operator<<(std::basic_ostream<O>& stream, TaggedID<I, T> id)
    {
        return stream << id.getID();
    };

    template<class I, class T>
    constexpr bool operator==(TaggedID<I, T> a, TaggedID<I, T> b) noexcept
    {
        return a.getID() == b.getID();
    };

    template<class I, class T>
    constexpr bool operator!=(TaggedID<I, T> a, TaggedID<I, T> b) noexcept
    {
        return a.getID() != b.getID();
    };

    //! <, defined so IDs can be used in ordered containers.
    template<class I, class T>
    constexpr bool operator<(TaggedID<I, T> a, TaggedID<I, T> b) noexcept
    {
        return a.getID() < b.getID();
    };
}

namespace std
{
    template<class T>
    struct hash;

    //! hash, defined so IDs can be used in unordered containers.
    template <class I, class T>
    struct hash<spatacs::TaggedID<I, T>>
    {
        size_t operator()(const spatacs::TaggedID<I, T>& x) const
        {
            return hash<I>{}( x.getID() );
        }
    };

}

#endif //SPATACS_ID_H
