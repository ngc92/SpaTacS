//
// Created by erik on 9/19/16.
//

#include <algorithm>
#include "ProjectileWeapon.h"
#include <boost/property_tree/ptree.hpp>
#include <iostream>

namespace spatacs
{
    namespace game
    {
        ProjectileWeapon::ProjectileWeapon(const IComponent::ptree& props):
                IWeapon(props),
                mDamage{ props.get<float>("HE_strength"),
                         props.get<float>("SO_strength"),
                         props.get<float>("AP_strength") },
                mPrecision( props.get<float>("precision") ),
                mRPM( props.get<float>("rpm") ),
                mMuzzleVelocity( props.get<speed_t>("muzzle_velocity") )
        {

        }

        ProjectileWeapon* ProjectileWeapon::clone() const
        {
            return new ProjectileWeapon(*this);
        }

        float ProjectileWeapon::strength(length_t, area_t) const
        {
            return 0;
        }

        void ProjectileWeapon::onStep(Starship& ship)
        {
            mReload -= 0.1;
        }

        bool ProjectileWeapon::ready() const
        {
            return mReload < 0;
        }

        boost::optional<ShotData> ProjectileWeapon::fire(const length_vec& delta_p, const velocity_vec& delta_v)
        {
            if(mReload > 0)
                return boost::none;

            // perfect aim
            speed_t vel = mMuzzleVelocity; // km/s
            auto d = length(delta_p);
            auto aim_pos = delta_p;

            for(unsigned i = 0; i < 3; ++i) {
                aim_pos = delta_p + (d / vel) * delta_v;
                auto old_d = d;
                d = length(aim_pos);
                //std::cout << "correct: " << i << " " << abs(d - old_d) << "\n";
                if( abs(d - old_d) < 10.0_m )
                    break;
            }



            // miss-aim
            length_t aim_radius = d / precision();
            auto aim_radius_miss = (rand() % 1001 / 1000.0) * aim_radius;

            auto miss_base = meters(rand() % 1001 - 500.0, rand() % 1001 - 500.0, rand() % 1001 - 500.0);
            auto miss = perpendicular(miss_base, aim_pos);

            length_t n = length(miss);
            if( n == 0.0_m ) miss *= 0.0;
            else miss *= aim_radius_miss / n;

            aim_pos += miss;

            d = length(aim_pos);
            mReload = 60 / mRPM;

            Damage dmg;
            if(mMode == HE_MODE)
            {
                dmg.high_explosive = mDamage.high_explosive;
            } else if(mMode == AP_MODE)
            {
                dmg.armour_piercing = mDamage.armour_piercing;
            } else if(mMode == SO_MODE)
            {
                dmg.shield_overload = mDamage.shield_overload;
            }

            return ShotData{aim_pos * vel/d, dmg};
        }

        float ProjectileWeapon::hit_chance(length_t distance, area_t xsec) const
        {
            auto aim_radius = distance / precision();
            length_t target_radius = sqrt(xsec);
            /// \todo test for distance == 0
            return std::min(1.0, (double)(target_radius / aim_radius));
        }

        double ProjectileWeapon::precision() const
        {
            return mPrecision * (status() + 1) * .5;
        }

        void ProjectileWeapon::setMode(std::uint64_t mode)
        {
            /// \todo fix that!
            if(mode < 3)
                mMode = Mode(mode);
        }

        uint64_t ProjectileWeapon::mode() const
        {
            return mMode;
        }
    }
}