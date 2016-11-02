//
// Created by erik on 9/21/16.
//

#include <iostream>
#include "Spawn.h"
#include "core/GameState.h"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include "game/Starship.h"
#include "game/Projectile.h"
#include "game/systems.h"
#include "physics/PhysicsWorld.h"

using namespace spatacs;
using namespace events;

SpawnProjectile::SpawnProjectile(game::ObjectID shooter, length_vec pos, velocity_vec vel, mass_t mass, length_t rad,
                                 game::Damage dmg) :
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

    auto proj = std::make_unique<game::Projectile>(game::ObjectID(new_id), mShooter, mDamage);
    proj->setPosition( mPosition );
    proj->setVelocity( mVelocity );
    proj->setMass( mMass );

    physics::Object obj(mPosition, mVelocity, mMass, new_id);
    obj.addFixture( mRadius );
    auto pid = context.world.spawn(obj);
    proj->setPhysicsID( pid );

    context.state.add(std::move(proj));
}



SpawnShip::SpawnShip(std::uint64_t team, std::string name, std::string type, const length_vec& position) :
    mTeam(team),
    mName(std::move(name)),
    mType(std::move(type)),
    mPosition(position)
{

}

namespace
{
    game::AmmoData getAmmoData(const std::string& type)
    {
        boost::property_tree::ptree tree;
        boost::property_tree::xml_parser::read_xml("data/ammunition.xml", tree);
        auto& source = tree.get_child("ammunition." + type);

        game::AmmoData data;
        data.mass   = source.get<mass_t>("mass");
        data.charge = source.get<energy_t>("energy");
        data.name   = type;
        data.damage.armour_pierce   = source.get<double>("AP", 0.0);
        data.damage.high_explosive  = source.get<double>("HE", 0.0);
        data.damage.shield_overload = source.get<double>("SO", 0.0);
        return data;
    }
}

void SpawnShip::apply(EventContext& context) const
{
    boost::property_tree::ptree tree;
    boost::property_tree::xml_parser::read_xml("data/"+mType+".xml", tree);
    auto data = tree.get_child("ship");
    auto mass = data.get<mass_t>("mass");
    game::ObjectID id{context.state.getNextFreeID()};

    auto ship = std::make_unique<game::Starship>(mTeam, mName, data);

    ship->setPosition( mPosition );
    ship->setMass( mass );
    ship->setVelocity( velocity_vec{0, 0, 0} );
    ship->setID(id);

    physics::Object obj(mPosition, velocity_vec{0,0,0}, mass, id.getID());
    obj.addFixture( ship->radius() ).setUserdata(0); // ship
    obj.addFixture( ship->radius() + 25.0_m ).setUserdata(1); // shield
    auto pid = context.world.spawn(obj);
    ship->setPhysicsID( pid );

    // now add the ammo
    for(auto& ammo : mAmmo)
    {
        ship->components().apply( game::AddAmmunition(getAmmoData(ammo.type), ammo.amount) );
    }

    auto add_fuel = core::make_system<game::ComponentEntity, game::FuelStorage>(
            [this](game::FuelStorage& fs)
            {
                fs.current = mFuel;
                /// \todo this does not take into accout capacity
                ///        and multiple tanks
            }
    );
    ship->components().apply(add_fuel);

    context.state.add(std::move(ship));
}

void SpawnShip::addAmmunition(std::string name, std::size_t amount)
{
    mAmmo.emplace_back( std::move(name), amount);
}

void SpawnShip::setFuel(mass_t f)
{
    mFuel = f;
}

SpawnShip::AmmoData::AmmoData(const std::string& type, size_t amount) : type(type), amount(amount)
{}
