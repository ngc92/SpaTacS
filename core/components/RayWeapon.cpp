//
// Created by erik on 9/20/16.
//

#include "RayWeapon.h"
#include <cmath>
#include "core/Projectile.h"

namespace spatacs
{
    namespace core
    {
        RayWeapon* RayWeapon::clone() const
        {
            return new RayWeapon(*this);
        }

        float RayWeapon::strength(length_t distance, float xsec) const
        {
            float shot_radius = distance.value / mDispLength;
            float hit = std::min(1.f, xsec / (shot_radius * shot_radius));
            return mStrength * hit;
        }

        void RayWeapon::onStep(Starship& ship)
        {
            if(mRecharging) {
                if(mCharge < mBurstLength)
                {
                    // total dmg: mBurstLength * 10 * mStrength
                    // total energy = dmg * egypdmg = mBurstLenght * factor
                    //  => factor =10*mStrength/dmgpegy
                    float factor = 10 * mStrength * mEnergyPerDamage;
                    float mx = mBurstLength * factor * 0.1f;
                    float energy = requestEnergy( std::min((mBurstLength - mCharge) * factor, mx));
                    mCharge += energy / factor;
                } else
                {
                    mRecharging = false;
                }
            }else {
                if (mCharge <= 0) {
                    mMissVector = { rand() % 1001 - 500, rand() % 1001 - 500, rand() % 1001 - 500};
                    mMissFactor = (rand() % 1001 / 1000.f);
                    mRecharging = true;
                } else
                {
                    mReady = true;
                }

            }

        }

        bool RayWeapon::ready() const
        {
            return mReady;
        }

        float RayWeapon::hit_chance(length_t distance, float xsec) const
        {
            auto aim_radius  = distance / precision();
            auto shot_radius = distance / mDispLength;

            // shot radius increases the effective radius of the target
            float effective_xsec = std::sqrt(xsec) + shot_radius.value;
            effective_xsec *= effective_xsec;

            float area = aim_radius.value * aim_radius.value;
            /// \todo test for distance == 0
            return std::min(1.f, effective_xsec / area);
        }

        double RayWeapon::precision() const
        {
            return mPrecision;
        }

        boost::optional<ShotData> RayWeapon::fire(const length_vec& delta_p, const velocity_vec& delta_v)
        {
            /*
            if(!mReady || mCharge <= 0 )
                return boost::none;

            // perfect aim
            float vel = 300; // km/s
            auto d = length(delta_p);
            auto aim_pos = delta_p + (d / vel) * delta_v;

            // miss-aim
            float aim_radius = d / precision();
            float aim_radius_miss = mMissFactor * aim_radius;

            auto miss = perpendicular(mMissVector, aim_pos);
            float n = length(miss);
            if( n == 0 ) miss *= 0;
            else miss *= aim_radius_miss / n;

            aim_pos += miss;

            d = length(aim_pos);
            mReady = false;
            mCharge -= 0.1;
            // time to disp length = mDispLength / vel
            float dps_ps = 1.f / (mDispLength / vel);
*/
            //return Projectile({0,0,0}, aim_pos * (vel / d), mStrength, 0.0_km, dps_ps);
        }

        void RayWeapon::setMode(std::uint64_t mode)
        {

        }

        uint64_t RayWeapon::mode() const
        {
            return 0;
        }
    }
}