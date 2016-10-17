//
// Created by erik on 9/21/16.
//

#ifndef SOI_ICOMPONENT_H
#define SOI_ICOMPONENT_H

#include <memory>
#include <boost/property_tree/ptree_fwd.hpp>
#include "comps.h"

namespace spatacs
{
    namespace game
    {
        class Starship;

        class IComponent
        {
        public:
            virtual ~IComponent() = default;

            // interaction with the energy system

            // hitpoints stuff
            /// get the current hit points
            double hp() const;
            /// get maximum hit points
            double max_hp() const;
            /// get the health status:
            double status() const;
            /// reduce hp.
            /// \return leftover damage.
            double dealDamage(double dmg);

            virtual IComponent* clone() const = 0;

            ComponentEntity& entity() { return mEntity; }

        protected:
            using ptree = boost::property_tree::ptree;

            IComponent( const ptree& data );

            ComponentEntity mEntity;
        };
    }
}

#endif //SOI_ICOMPONENT_H
