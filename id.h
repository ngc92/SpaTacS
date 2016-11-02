//
// Created by erik on 11/1/16.
//

#ifndef SPATACS_ID_H
#define SPATACS_ID_H

#include <iosfwd>

namespace spatacs
{
    template<class Int, class Tag>
    class TaggedID
    {
    public:
        constexpr TaggedID() = default;
        constexpr explicit TaggedID(Int i) : mValue(i) {};
        constexpr Int getID() const { return mValue; }
        constexpr explicit operator bool() const { return mValue != 0; }
        constexpr void reset() { mValue = 0; }
    private:
        Int mValue{0};
    };

    template<class O, class I, class T>
    std::basic_ostream<O>& operator<<(std::basic_ostream<O>& stream, TaggedID<I, T> id)
    {
        return stream << id.getID();
    };

    template<class I, class T>
    bool operator==(TaggedID<I, T> a, TaggedID<I, T> b)
    {
        return a.getID() == b.getID();
    };

    template<class I, class T>
    bool operator!=(TaggedID<I, T> a, TaggedID<I, T> b)
    {
        return a.getID() != b.getID();
    };

    template<class I, class T>
    bool operator<(TaggedID<I, T> a, TaggedID<I, T> b)
    {
        return a.getID() < b.getID();
    };
}

namespace std
{
    template<class T>
    struct hash;

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
