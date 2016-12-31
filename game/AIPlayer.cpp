//
// Created by erik on 9/23/16.
//

#include "AIPlayer.h"
#include <boost/range/adaptor/indirected.hpp>
#include <events/Combat.h>
#include <iostream>
#include "game/Starship.h"
#include "cmd/CommandManager.h"
#include "systems/Ammunition.h"
#include "SubSystems.h"
#include "GameState.h"

using namespace spatacs;
using namespace game;

void AIPlayer::init()
{
    mState = std::make_shared<game::GameState>();
}

void AIPlayer::setState(state_t state)
{
    for(auto& obj : *state)
    {
        if(!dynamic_cast<const game::Starship*>(&obj))
        {
            continue;
        }
        auto& own = dynamic_cast<const game::Starship&>(obj);

        if(own.team() != mOwnTeam || !own.alive())
            continue;

        // if the shield is damaged.
        double own_shield = own.shield_strength(*state);

        length_t min = 100.0_km;
        // find closest ship to attack
        const game::Starship* target = nullptr;
        for (auto& e : *state)
        {
            auto eship = dynamic_cast<const game::Starship*>(&e);
            if(!eship || !eship->alive())
                continue;

            if (eship->team() != own.team() && distance(e, own) < min) {
                min = distance(e, own);
                target = eship;
            }
        }

        if(!target)
            continue;

        // if found, do attack
        if(min < 20.0_km)  {
            mCommands->addCommand( own.id(), cmd::Attack(target->id()) );
            auto best = getBestAmmo(own, *target, target->shield_strength( *state ));
            auto wpn_id_range = own.components().get_matching_ids(core::type_v<systems::signatures::AimSignature>);
            for(const auto& id : wpn_id_range)
            {
                mCommands->addCommand(own.id(), cmd::SetWpnMode(own.id(), id, best.ammo));
            }
        }

        // fly closer if shield is stronger
        if(own_shield > target->shield_strength( *state ) || own_shield > 2)
        {
            mCommands->addCommand( own.id(),
                                   cmd::Move(std::make_unique<cmd::movement::EngageTarget>(
                                           target->id(), 0.3_kps))
            );
        } else
        {
            auto delta = own.position() - target->position();
            delta *= 1.0_km / length(delta);
            mCommands->addCommand( own.id(),
                                   cmd::Move(std::make_unique<cmd::movement::FollowRoute>(
                                           own.position() + delta, 2.0_kps))
            );
        }
    }

    mState = state;
}

auto AIPlayer::getBestAmmo(const Starship& own, const Starship& target, double target_shield) const -> BestAmmo
{
    systems::ListAmmunition la;
    own.components().apply(la);

    double best = 0.0;
    std::string mode = "";
    for(auto& ammo : la)
    {
        if(ammo.amount < 1)
            continue;

        Damage estimated = ammo.data.damage;
        estimated.kinetic      = ammo.data.charge / 20000.0_kJ;  /// \todo magic constant here
        auto sd = getShieldDamage(estimated, target_shield);
        auto ad = getArmourDamage(sd.remaining, target.armour(), target.radius());
        // give only 20% weight to damaging shield and 0.75% for damaging armour
        double total = 0.20 * sd.applied + 0.75 * ad.applied + ad.remaining.kinetic + ad.remaining.high_explosive;
        if( total > best )
        {
            best = total;
            mode = ammo.data.name;
        }
    }
    return {mode, best};
}

bool AIPlayer::step()
{
    return false;
}

AIPlayer::AIPlayer(std::uint64_t team, std::shared_ptr<cmd::CommandManager> cmd) :
        mOwnTeam( team ),
        mCommands( std::move(cmd) )
{
}
