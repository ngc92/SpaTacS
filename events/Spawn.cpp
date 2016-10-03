//
// Created by erik on 9/21/16.
//

#include <iostream>
#include "Spawn.h"
#include "core/GameState.h"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

using namespace spatacs;
using namespace events;

SpawnProjectile::SpawnProjectile(std::uint64_t shooter, length_vec pos, velocity_vec vel, mass_t mass, length_t rad,
                                 core::Damage dmg) :
    mShooter(shooter), mPosition(pos), mVelocity(vel), mMass(mass), mRadius(rad), mDamage(dmg)
{

}

void SpawnProjectile::apply(EventContext& context) const
{
    auto new_id = context.state.getNextFreeID();

    core::Projectile proj(new_id, mShooter, mDamage);

    context.state.addProjectile( std::move(proj) );
    physics::events::Spawn sp{mPosition, mVelocity,
                              mMass, mRadius,
                              new_id, 0.0_s};
    context.world.pushEvent(std::move(sp));
}



SpawnShip::SpawnShip(std::uint64_t team, std::string name, const length_vec& position) :
    mTeam(team),
    mName(std::move(name)),
    mPosition(position)
{

}

void SpawnShip::apply(EventContext& context) const
{
    boost::property_tree::ptree tree;
    boost::property_tree::xml_parser::read_xml("data/ship.xml", tree);

    auto ship = core::Starship(mTeam, mName, tree);
    auto id = context.state.getNextFreeID();
    ship.setID(id);
    context.state.addShip(std::move(ship));
    context.world.pushEvent(physics::events::Spawn{mPosition, velocity_vec{0, 0, 0}, 65000.0_kg, 25.0_m, id, 0.0_s});
}
