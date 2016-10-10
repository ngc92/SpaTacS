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
    mShooter(shooter),
    mPosition(pos),
    mVelocity(vel),
    mMass(mass),
    mRadius(rad),
    mDamage(dmg)
{

}

void SpawnProjectile::apply(EventContext& context) const
{
    auto new_id = context.state.getNextFreeID();

    core::Projectile proj(new_id, mShooter, mDamage);
    proj.setPosition( mPosition );
    proj.setVelocity( mVelocity );
    proj.setMass( mMass );

    physics::Object obj(mPosition, mVelocity, mMass, new_id);
    obj.addFixture( mRadius );
    auto pid = context.world.spawn(obj);
    proj.setPhysicsID( pid );

    context.state.addProjectile( std::move(proj) );
}



SpawnShip::SpawnShip(std::uint64_t team, std::string name, std::string type, const length_vec& position) :
    mTeam(team),
    mName(std::move(name)),
    mType(std::move(type)),
    mPosition(position)
{

}

void SpawnShip::apply(EventContext& context) const
{
    boost::property_tree::ptree tree;
    boost::property_tree::xml_parser::read_xml("data/"+mType+".xml", tree);
    auto mass = tonnes(tree.get<double>("mass"));
    auto id = context.state.getNextFreeID();

    auto ship = core::Starship(mTeam, mName, tree);

    ship.setPosition( mPosition );
    ship.setMass( mass );
    ship.setVelocity( velocity_vec{0, 0, 0} );
    ship.setID(id);

    std::cout << "spawn " << id << " " << ship.id() << "\n";

    physics::Object obj(mPosition, velocity_vec{0,0,0}, mass, id);
    obj.addFixture( 25.0_m );
    auto pid = context.world.spawn(obj);
    ship.setPhysicsID( pid );

    context.state.addShip(std::move(ship));
}
