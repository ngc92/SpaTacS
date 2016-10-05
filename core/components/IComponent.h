//
// Created by erik on 9/21/16.
//

#ifndef SOI_ICOMPONENT_H
#define SOI_ICOMPONENT_H

#include <memory>
#include <boost/property_tree/ptree_fwd.hpp>

namespace spatacs
{
    namespace core
    {
        class Starship;

        class IComponent
        {
        public:
            virtual ~IComponent() = default;

            virtual void onStep(Starship& ship)
            { };

            // interaction with the energy system

            /// energy step. Returns the energy that remains in the cache and empties it.
            double getExcessEnergy() const;

            /// gets the amount of energy this system requests.
            double getEnergyRequest() const;

            /// provide energy to system, put it into the cache
            void provideEnergy(double offer);

            /// gets the priority for energy supply to this component
            float energyPriority() const;

            // hitpoints stuff

            /// get the current hit points
            float hp() const;
            /// get maximum hit points
            float max_hp() const;
            /// get the health status:
            double status() const;
            /// reduce hp.
            /// \return leftover damage.
            float dealDamage( float dmg );

            // user interaction
            /// sets the priority for energy supply to this component.
            /// If p <= 1, p is set to 1.
            void setEnergyPriority( float p );

            virtual IComponent* clone() const = 0;

        protected:
            using ptree = boost::property_tree::ptree;

            IComponent( const ptree& data );

            /// request \p amount energy from the cache. Returns
            /// the amount that can be supplied.
            double requestEnergy(double amount);

            /// places produced energy into the cache.
            void produceEnergy(double amount);
        private:
            // this comes in handy for basically all components, so we put this code here.
            double mEnergyCache      = 0;
            double mLastTotalRequest = 0;
            float mEnergyPriority    = 1;

            float mMaxHitPoints;
            float mHitPoints;
        };
    }
}

#endif //SOI_ICOMPONENT_H
