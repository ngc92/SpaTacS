//
// Created by erik on 11/1/16.
//

#ifndef SPATACS_ID_H
#define SPATACS_ID_H

namespace spatacs
{
    template<class Int, class Tag>
    class TaggedID
    {
    public:
        TaggedID() = default;
        explicit TaggedID(Int i) : mValue(i) {};
        Int getID() const { return mValue; }
    private:
        Int mValue{0};
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

#endif //SPATACS_ID_H
