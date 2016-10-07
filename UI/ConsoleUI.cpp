//
// Created by erik on 9/19/16.
//

#include "ConsoleUI.h"
#include <thread>
#include <iomanip>
#include <boost/io/ios_state.hpp>
#include "core/components/ShieldGenerator.h"
#include <iostream>
#include "core/components/IWeapon.h"

namespace spatacs {
    namespace ui {
        void ConsoleUI::init()
        {
            mConsoleThread = std::thread([this](){ readLoop(); });
        }

        std::vector<cmd::Command> ConsoleUI::getCommands() const
        {
            std::lock_guard<std::mutex> lck(mCmdMutex);
            return mCommandManager.getCommands();
        }

        void ConsoleUI::setState(const core::GameState& state)
        {
            std::lock_guard<std::mutex> lck(mStateMutex);
            mState = state;
            // remove commands of dead ships
            std::lock_guard<std::mutex> lck2(mCmdMutex);
            mCommandManager.validate(state);
        }

        bool ConsoleUI::step()
        {
            return true;
        }

        void ConsoleUI::readLoop()
        {
            while (true) {
                std::string command;
                std::cin >> command;
                if (command == "move") {
                    addCommand(cmd::Move(std::cin));
                } else if (command == "attack") {
                    addCommand(cmd::Attack(std::cin));
                }  else if (command == "info") {
                    std::lock_guard<std::mutex> lck(mStateMutex);
                    printState(mState);
                } else if (command == "commands") {
                    for (auto& cd : mCommandManager.getCommands()) {
                        std::cout << cd << "\n";
                    }
                } else if (command == "undo")
                {
                    /// \todo add possibility to remove commands.
                    /*
                    std::lock_guard<std::mutex> lck(mCmdMutex);
                    int u = 0;
                    std::cin >> u;
                    if(u < mCommands.size())
                        mCommands.erase( begin(mCommands) + u );
                     */
                } else if (command == "chance" ) {
                    boost::io::ios_all_saver ias( std::cout );
                    std::cout << std::setprecision(2) << std::fixed;

                    std::lock_guard<std::mutex> lck(mStateMutex);
                    std::uint64_t shooter, target;
                    std::cin >> shooter >> target;
                    const auto& ss = mState.getShip(shooter);
                    const auto& st = mState.getShip(target);
                    for(unsigned i = 0; i < ss.weapon_count(); ++i) {
                        float hc = ss.weapon(i).hit_chance(distance(ss, st), st.radius() * st.radius());
                        float hd = ss.weapon(i).strength(distance(ss, st), st.radius() * st.radius());
                        std::cout << " hit chance " << 100 * hc << "% for ";
                        std::cout << hd << " ( "<< int(100*hc * hd) / 100.f <<" )\n";
                    }
                } else if (command == "ship-info" ) {
                    boost::io::ios_all_saver ias( std::cout );
                    std::cout << std::setprecision(2) << std::fixed;
                    auto p1 = std::setprecision(1);
                    std::lock_guard<std::mutex> lck(mStateMutex);
                    std::uint64_t ship;
                    std::cin >> ship;
                    const auto& ss = mState.getShip(ship);
                    std::cout << "Ship " << ss.id() << ":\n";
                    std::cout << " pos:  " << ss.position() << "\n";
                    std::cout << " HP:   " << ss.hp() << "\n";
                    std::cout << " team: " << ss.team() << "\n";
                    std::cout << " shield: " << ss.shield().shield() << "\n";
                    std::cout << p1;
                    for(unsigned i = 0; i < ss.weapon_count(); ++i) {
                        std::cout << " wpn (100m² target):\n";
                        std::cout << "  precision 500m: " << ss.weapon(i).hit_chance(500.0_m, 10.0_m * 10.0_m) * 100 << "%\n";
                        std::cout << "  strength  500m: " << ss.weapon(i).strength(500.0_m,  10.0_m * 10.0_m) << "\n";
                        std::cout << "  precision 2km:  " << ss.weapon(i).hit_chance(2.0_km,  10.0_m * 10.0_m) * 100 << "%\n";
                        std::cout << "  strength  2km:  " << ss.weapon(i).strength(2.0_km,  10.0_m * 10.0_m) << "\n";
                    }
                } else if (command == "exit")
                {
                    break;
                }
            }
        }


        void ConsoleUI::printState(const core::GameState& state)
        {
            auto sp = std::setprecision(2);
            std::cout << "ships: \n";
            for (auto& ship : state.getShips()) {
                std::cout << ship.id() << ": " << ship.team() << " " << sp << ship.hp() << " @ " << ship.position()
                          << "\n";
            }
        }

        void ConsoleUI::addCommand( cmd::Command c )
        {
            std::lock_guard<std::mutex> lck(mCmdMutex);
            mCommandManager.addCommand( std::move(c) );
        }

        void ConsoleUI::notifyEvents(const std::vector<std::unique_ptr<events::IEvent>>& events)
        {

        }
    }
}