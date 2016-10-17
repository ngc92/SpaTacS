//
// Created by erik on 9/18/16.
//

#ifndef SOI_ENGINE_H
#define SOI_ENGINE_H

#include "IComponent.h"
#include "physics/units.h"

namespace spatacs
{
namespace game
{
    class Engine : public IComponent
    {
    public:
        Engine() = default;
        Engine(const ptree& props);

        virtual Engine* clone() const override;
    };
}
}

#endif //SOI_ENGINE_H
