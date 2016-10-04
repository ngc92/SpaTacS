#ifndef SOI_STARSHIP_H_INCLUDED
#define SOI_STARSHIP_H_INCLUDED

#include <cstdint>
#include <memory>
#include <vector>
#include <boost/property_tree/ptree_fwd.hpp>

#include "vec.h"
#include "physics/Object.h"
#include "GameObject.h"


namespace spatacs
{
    namespace core {
        class Engine;
        class ShieldGenerator;
        class Hull;
        class IWeapon;
        class PowerPlant;

        struct SystemStatus
        {
            float current;
            float max;
        };

        /// the purpose of this class is the ability to add members to
        /// Starship, without having to worry about adding them to the
        /// copy/move c'tors.
        class ShipData
        {
        public:
            /// Ship info
            uint64_t team() const;
            const std::string& name() const;

            float max_hp() const;
        protected:
            ShipData() = default;
            ShipData(std::uint64_t team, std::string name);
            ~ShipData() = default;

            float mHitPoints = 10;
            float mMaxHitPoints = 10;

            // Energy management status
            double mEnergyUsed     = 0;
            double mEnergyProduced = 0;

        private:
            std::uint64_t mTeam = 0;
            std::string mName;
        };

        class Starship : public GameObject, public ShipData {
        public:
            Starship();
            Starship( Starship&& );
            explicit Starship(const Starship& other );

            Starship& operator=( Starship&& );
            Starship& operator=( const Starship& other );

            ~Starship();

            // Ship from definition
            Starship(std::uint64_t team, std::string name, const boost::property_tree::ptree& data);

            float HP() const;
            bool alive() const;

            /// called at the end of a game step.
            void onStep();
            void setHP( float hp );

            /// energy
            double producedEnergy() const;
            double usedEnergy() const;

            /// Subcomponents
            // engine interface
            const Engine& engine() const;
            Engine& getEngine();

            // Shield interface
            SystemStatus shield_strength() const;
            const ShieldGenerator& shield() const;
            ShieldGenerator& getShield();

            // hull interface
            SystemStatus hull_status() const;
            const Hull& hull() const;
            Hull& getHull();

            // weapon interface
            std::size_t weapon_count() const;
            const IWeapon& weapon( std::size_t id) const;
            IWeapon& getWeapon( std::size_t id );


            // damage the ship
            void dealDamage( float dmg );

        private:
            // components
            struct SubSystems;
            std::unique_ptr<SubSystems> mSubSystems;
        };

        length_t distance(const Starship& s1, const Starship& s2);
    }
}

#endif
