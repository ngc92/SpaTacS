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
        class IWeapon;
        class PowerPlant;
        class FuelTank;
        class SubSystems;

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

            /// energy
            double producedEnergy() const;
            double usedEnergy() const;

            // hitpoints
            double hp() const;
            double max_hp() const;
            void setHP( double hp );

            double armour() const;
            double max_armour() const;
            void setArmour( double new_value );

            length_t radius() const;
            void setRadius(length_t radius);

        protected:
            ShipData() = default;
            ShipData(std::uint64_t team, std::string name, const boost::property_tree::ptree& data);
            ~ShipData() = default;

            // structure
            double mMaxHitPoints = 10;
            double mHitPoints    = 10;

            // armour
            double mMaxArmour = 10;
            double mCurArmour = 10;

            // Energy management status
            double mEnergyUsed     = 0;
            double mEnergyProduced = 0;

            length_t mRadius;

        private:
            std::uint64_t mTeam = 0;
            std::string   mName;
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

            bool alive() const override;
            Starship* clone() const override;

            /// called at the end of a game step.
            void onStep() override;

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

            // weapon interface
            std::size_t weapon_count() const;
            const IWeapon& weapon( std::size_t id) const;
            IWeapon& getWeapon( std::size_t id );

            // tank interface
            const FuelTank& tank() const;
            FuelTank& getTank();

            // damage the ship
            void dealDamage( float dmg );

        private:
            // components
            std::unique_ptr<SubSystems> mSubSystems;
        };
    }
}

#endif
