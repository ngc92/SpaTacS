//
// Created by erik on 10/18/16.
//

#include "LevelLoader.h"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

using namespace spatacs;
using namespace ui;
using boost::property_tree::ptree;

void LevelLoader::getCommandEvents(std::vector<events::EventPtr>& evts)
{
    for(auto& ev : mSpawnEvents)
        evts.push_back( std::make_unique<events::SpawnShip>(ev) );

    mSpawnEvents.clear();

}

LevelLoader::LevelLoader(std::string filename)
{
    boost::property_tree::ptree level;
    boost::property_tree::xml_parser::read_xml(filename, level);
    for(auto& ship : level.get_child("level"))
    {
        if(ship.first == "ship") {
            std::string name = ship.second.get<std::string>("name");
            std::string type = ship.second.get<std::string>("type");
            std::uint8_t team = ship.second.get<std::uint8_t>("<xmlattr>.team");
            auto x = ship.second.get<length_t>("position.<xmlattr>.x");
            auto y = ship.second.get<length_t>("position.<xmlattr>.y");
            auto z = ship.second.get<length_t>("position.<xmlattr>.z");

            mSpawnEvents.emplace_back(team, name, type, length_vec{x, y, z});

            for(auto& sub : ship.second)
            {
                if(sub.first == "ammo")
                {
                    mSpawnEvents.back().addAmmunition(sub.second.get<std::string>("type"),
                                                      sub.second.get<std::size_t>("amount"));
                }
            }


        }
    }
}

bool LevelLoader::step()
{
    return false;
}

void LevelLoader::setState(const std::shared_ptr<const core::GameState>& state)
{
}
