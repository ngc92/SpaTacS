//
// Created by erik on 9/20/16.
//

#ifndef SOI_HULL_H
#define SOI_HULL_H

#include "IComponent.h"

namespace spatacs
{
namespace core
{
    class Hull : public IComponent
    {
    public:
        Hull(const ptree& data);

        Hull* clone() const override;

        void onStep();

        float max_armour() const;
        float armour() const;

        void setArmour( float new_value );

    private:
        float mMaxArmour;
        float mCurArmour;
    };
}
}


#endif //SOI_HULL_H
