//
// Created by erik on 12/10/16.
//

#include "ShieldEvent.h"
#include "game/GameState.h"
#include "game/Starship.h"

using namespace spatacs;
using namespace events;
using namespace game;

ShieldEvent::ShieldEvent(game::ObjectID target, double change) :
        mShieldObject(target),
        mChange(change)
{

}

void ShieldEvent::apply(EventContext &context) const
{
    auto& target = context.state.getObject(mShieldObject);
    auto& casted = dynamic_cast<game::Starship&>(target);
    casted.setShield( casted.shield() + mChange );
}
