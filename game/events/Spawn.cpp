//
// Created by erik on 1/2/17.
//

#include "Spawn.h"
#include "game/State.h"
#include "game/entity/entity.h"
#include "physics/PhysicsWorld.h"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

using namespace spatacs;
using namespace game;
using namespace events;


Spawn::Spawn(const length_vec& pos, const velocity_vec& vel) : mPosition(pos), mVelocity(vel)
{

}

GameEntityHandle Spawn::spawn(EventContext& context) const
{
    return context.state.entity_manager().create();
}


SpawnProjectile::SpawnProjectile(GameEntityID shooter, length_vec pos, velocity_vec vel, mass_t mass, length_t rad,
                                 game::Damage dmg) :
        Spawn(pos, vel),
        mShooter(shooter),
        mMass(mass),
        mRadius(rad),
        mDamage(dmg)
{

}

void SpawnProjectile::apply(EventContext& context) const
{
    GameEntityHandle entity = spawn(context);
    entity.add<components::Timer>(10.0_s);
    entity.add<components::Parent>(mShooter);
    entity.add<components::Warhead>(mDamage);

    physics::Object obj(mPosition, mVelocity, mMass, entity.id().getID());
    obj.addFixture( mRadius );
    auto pid = context.world.spawn(obj);

    entity.add<components::PhysicsData>(pid, mMass, mPosition, mVelocity);
}

SpawnShip::SpawnShip(std::uint64_t team, std::string name, std::string type, const length_vec& position) :
        Spawn(position, velocity_vec{0,0,0}),
        mTeam(team),
        mName(std::move(name)),
        mType(std::move(type))
{

}
/*
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
*/
void SpawnShip::apply(EventContext& context) const
{
    boost::property_tree::ptree tree;
    boost::property_tree::xml_parser::read_xml("data/"+mType+".xml", tree);
    auto data = tree.get_child("ship");
    auto mass = data.get<mass_t>("mass");


    GameEntityHandle entity = spawn(context);
    entity.add<components::Name>(mName);
    entity.add<components::Affiliation>(mTeam);

    physics::Object obj(mPosition, velocity_vec{0, 0, 0}, mass, entity.id().getID());
    /*obj.addFixture( ship->radius() ).setUserdata(0); // ship
    obj.addFixture( ship->radius() + 25.0_m ).setUserdata(1); // shield
     */
    auto pid = context.world.spawn(obj);

    entity.add<components::PhysicsData>(pid, mass, mPosition, mVelocity);


    /*
    // now add the ammo
    for(auto& ammo : mAmmo)
    {
        ship->components().apply( game::systems::AddAmmunition(getAmmoData(ammo.type), ammo.amount) );
    }

    mass_t rest = game::systems::fill_fuel(ship->components(), mFuel);
    if( rest > 0.0_kg )
        std::cerr << "Could not fit total fuel into tank!" << std::endl;
     */
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
