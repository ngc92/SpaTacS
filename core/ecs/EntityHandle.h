//
// Created by erik on 11/20/16.
//

#ifndef SPATACS_CORE_ENTITYHANDLE_H
#define SPATACS_CORE_ENTITYHANDLE_H

namespace spatacs
{
namespace core
{
namespace ecs
{

    /*! \brief A handle to an entity.
     *  \details All data of entities is saved inside
     *      the entity manager. This handle provides
     *      a convenient way of accessing that data.
     */
    template<class Config>
    class EntityHandle
    {
        using manager_t = typename Config::manager_t;
        using id_t      = typename Config::id_t;
    public:
        EntityHandle() noexcept = default;
        EntityHandle(manager_t& mgr, id_t id) : mManager(&mgr), mID(id) { }

        //! gets the ID of this entity.
        id_t id() const noexcept { return mID; }

        template<class T>
        bool has() const;

        template<class T>
        T& get();

        template<class T>
        const T& get() const;

        /// Add a new component of type \p T, which is constructed with \p args.
        template<class T, class... Args>
        T& add(Args&&... args);

        template<class T>
        void remove();

        bool alive() const;
        void kill();
    private:
        manager_t* mManager{nullptr};
        id_t mID{};
    };

    template<class T>
    bool EntityHandle<T>::alive() const
    {
        return mManager->is_alive(mID);
    }

    template<class T>
    void EntityHandle<T>::kill()
    {
        mManager->kill(mID);
    }

    template<class Config>
    template<class T>
    bool EntityHandle<Config>::has() const
    {
        return mManager->template has_component<T>(mID);
    }

    template<class Config>
    template<class T>
    T& EntityHandle<Config>::get()
    {
        return mManager->template get_mutable_component<T>(mID);
    }

    template<class Config>
    template<class T>
    const T& EntityHandle<Config>::get() const
    {
        return mManager->template get_component<T>(mID);
    }

    template<class Config>
    template<class T, class... Args>
    T& EntityHandle<Config>::add(Args&&... args)
    {
        return mManager->template add_component<T>(mID, std::forward<Args>(args)...);
    }

    template<class C>
    template<class T>
    void EntityHandle<C>::remove()
    {
        mManager->template remove_component<T>(mID);
    }
}
}
}

#endif //SPATACS_CORE_ENTITYHANDLE_H
