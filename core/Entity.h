//
// Created by erik on 10/7/16.
//

#ifndef SPATACS_CORE_ENTITY_H
#define SPATACS_CORE_ENTITY_H

#include <bitset>
#include <tuple>
#include <type_traits>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/begin.hpp>
#include <boost/mpl/find.hpp>
#include <boost/throw_exception.hpp>

namespace spatacs
{
    namespace core
    {
        template<class... Components>
        class Entity
        {
            using types = boost::mpl::vector<Components...>;
            static constexpr std::size_t comp_count = sizeof...(Components);

        public:
            template<class T>
            static constexpr std::size_t index()
            {
                using simple = typename std::decay<T>::type;
                using iter_t = typename boost::mpl::find<types, simple>::type;
                static_assert(!std::is_same<iter_t, typename boost::mpl::end<types>::type>::value, "trying to get invalid index");
                return boost::mpl::distance<typename boost::mpl::begin<types>::type, iter_t>::value;
            }

            template<class T>
            static constexpr std::size_t index_v = index<T>();

            using bits_t = std::bitset<comp_count+1>;


            explicit Entity(std::uint64_t id) : mID( id ) { }

            std::uint64_t id() const { return mID; }

            template<class T>
            bool has() const { return mBits.test( index<T>() ); }

            template<class T>
            T& get()
            {
                if(has<T>())
                {
                    return std::get<index_v<T>>(mData);
                } else
                {
                    BOOST_THROW_EXCEPTION( std::logic_error("Trying to access invalid component") );
                }
            }

            template<class T>
            const T& get() const
            {
                if(has<T>())
                {
                    return std::get<index_v<T>>(mData);
                } else
                {
                    BOOST_THROW_EXCEPTION( std::logic_error("Trying to access invalid component") );
                }
            }

            template<class T>
            T& add(T&& data)
            {
                mBits.set(index_v<T>);
                auto& c = std::get<index_v<T>>(mData);
                c = std::move(data);
                return c;
            }

            template<class T, class... Args>
            T& add(Args&&... args)
            {
                mBits.set(index_v<T>);
                auto& c = std::get<index_v<T>>(mData);
                c = T(std::forward<Args>(args)...);
                return c;
            }

            template<class T>
            void remove()
            {
                mBits.reset(index_v<T>);
            }

            const bits_t& bits() const { return mBits; }

            bool alive() const
            {
                return mBits.test(comp_count);
            }

            void kill()
            {
                mBits.reset(comp_count);
            }

        private:
            std::uint64_t mID;
            bits_t        mBits;
            std::tuple<Components...> mData;
        };
    }
}

#endif //SPATACS_CORE_ENTITY_H