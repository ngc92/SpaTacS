//
// Created by erik on 11/28/16.
//

#ifndef SPATACS_CORE_TYPEDGAMEINTERFACE_H
#define SPATACS_CORE_TYPEDGAMEINTERFACE_H

#include "GameInterface.h"
#include <type_traits>

namespace spatacs
{
namespace core
{
    template<class State, class Notification>
    class TypedGameInterface : public GameInterface
    {
    //! \note putting in the static assert here would require State to be a complete type at the point of instantiation
    //! of TypedGameInterface.
//        static_assert( std::is_base_of<GameStateBase, State>::value, "State type has to be derived from core::GameStateBase" );
    public:
    protected:
        using base_state_t  = GameInterface::state_t;
        using base_notify_t = GameInterface::notify_t;
        using state_t       = std::shared_ptr<const State>;
        using notify_t      = std::vector<Notification>;

        virtual void setState( state_t state ) { }
        virtual void notify(const notify_t& notify) { }
    private:
        void setState(const base_state_t& state) final
        {
            setState( std::dynamic_pointer_cast<const State>(state) );
        }

        void notify(const base_notify_t& events) final
        {
            notify( events.get_as<Notification>() );
        }
    };
}
}

#endif //SPATACS_CORE_TYPEDGAMEINTERFACE_H
