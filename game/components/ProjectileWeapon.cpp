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
                IWeapon(props)
        {
            mEntity.add<WeaponAimData>(props.get<speed_t>("muzzle_velocity"), props.get<double>("precision"));
            auto& pwd = mEntity.add<ProjectileWpnData>();
            pwd.mDamage = Damage{ props.get<float>("HE_strength"),
                            props.get<float>("SO_strength"),
                            props.get<float>("AP_strength") };
            pwd.mRPM = props.get<float>("rpm");
            mEntity.add<Timer>();
        }

        ProjectileWeapon* ProjectileWeapon::clone() const
        {
            return new ProjectileWeapon(*this);
        }

        void ProjectileWeapon::onStep(Starship& ship)
        {
        }

        bool ProjectileWeapon::ready() const
        {
            return mEntity.get<Timer>().time <= 0;
        }

        boost::optional<ShotData> ProjectileWeapon::fire(const length_vec& delta_p, const velocity_vec& delta_v)
        {
            if(!ready())
                return boost::none;

            // perfect aim
            speed_t vel = mEntity.get<WeaponAimData>().muzzle_velocity; // km/s
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
            mEntity.get<Timer>().time = 60 / mEntity.get<ProjectileWpnData>().mRPM;

            Damage dmg;
            if(mEntity.get<ProjectileWpnData>().mMode == ProjectileWpnData::HE_MODE)
            {
                dmg.high_explosive = mEntity.get<ProjectileWpnData>().mDamage.high_explosive;
            } else if(mEntity.get<ProjectileWpnData>().mMode == ProjectileWpnData::AP_MODE)
            {
                dmg.armour_piercing = mEntity.get<ProjectileWpnData>().mDamage.armour_piercing;
            } else if(mEntity.get<ProjectileWpnData>().mMode == ProjectileWpnData::SO_MODE)
            {
                dmg.shield_overload = mEntity.get<ProjectileWpnData>().mDamage.shield_overload;
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
            return mEntity.get<WeaponAimData>().precision * (status() + 1) * .5;
        }

        void ProjectileWeapon::setMode(std::uint64_t mode)
        {
            /// \todo fix that!
            if(mode < 3)
                mEntity.get<ProjectileWpnData>().mMode = ProjectileWpnData::Mode(mode);
        }

    }
}