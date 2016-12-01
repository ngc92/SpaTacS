//
// Created by erik on 11/28/16.
//

#ifndef SPATACS_ANY_VECTOR_H
#define SPATACS_ANY_VECTOR_H

#include <memory>
#include <vector>

namespace spatacs
{
namespace core
{
    /*! \brief Class that allows storing a vector<T>, where T is only known at runtime.
     *  \details This class is like boost::any, but it restricts the possibly contained
     *          values to (potentially move-only) std::vectors.
     *          The typed vector can be accessed by the \p get_as methods.
     */
    class any_vector
    {
    public:
        // constructors.
        any_vector() = default;

        template<class T>
        explicit any_vector(std::vector<T> source) :
                mData( std::make_unique<SavedVector<T>>(std::move(source)) )
        {

        }

        // access typed data.
        template<class T>
        std::vector<T>& get_as() { return dynamic_cast<SavedVector<T>&>(*mData).vec; }

        template<class T>
        const std::vector<T>& get_as() const { return dynamic_cast<const SavedVector<T>&>(*mData).vec; }

    private:
        // implementation detail classes for type erasure.
        struct VectorHolder { virtual ~VectorHolder() = default; };

        template<class T>
        struct SavedVector : public VectorHolder
        {
            SavedVector( std::vector<T> v ) : vec( std::move(v) ) { }
            std::vector<T> vec;
        };

        std::unique_ptr<VectorHolder> mData;
    };
}
}

#endif //SPATACS_ANY_VECTOR_H
