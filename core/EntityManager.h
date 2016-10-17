//
// Created by erik on 10/8/16.
//

#ifndef SPATACS_MANAGER_H
#define SPATACS_MANAGER_H

#include <cstdint>
#include <map>
#include "System.h"

namespace spatacs
{
    namespace core
    {
        template<class T>
        class EntityManager
        {
        public:
            template<class... Args>
            void apply( System<T, Args...>&& system );

            template<class... Args>
            void apply( System<T, Args...>&& system ) const;

            bool has(std::uint64_t id)
            {
                return mEntities.count(id) == 1;
            }

            T& addEntity()
            {
                auto id = ++mFreeID;
                auto r = mEntities.emplace(id, T(id));
                return r.first->second;
            }

            T& get(std::uint64_t id)
            {
                return mEntities.at(id);
            }

            const T& get(std::uint64_t id) const
            {
                return mEntities.at(id);
            }

            /// remove all entities that are no longer alive.
            void cleanup();

        private:
            std::map<uint64_t, T> mEntities;
            std::uint64_t mFreeID = 0;
        };

        template<class T>
        template<class... Args>
        void EntityManager<T>::apply( System<T, Args...>&& system )
        {
            for(auto& e : mEntities)
            {
                system(e.second);
            }
        };

        template<class T>
        template<class... Args>
        void EntityManager<T>::apply( System<T, Args...>&& system ) const
        {
            for(auto& e : mEntities)
            {
                system(e.second);
            }
        }

        template<class T>
        void EntityManager<T>::cleanup()
        {
            for(auto b = mEntities.begin(); b != mEntities.end();)
            {
                if(!b->second.alive())
                {
                    b = mEntities.erase(b);
                } else
                {
                    ++b;
                }
            }
        };
    }
}

#endif //SPATACS_MANAGER_H