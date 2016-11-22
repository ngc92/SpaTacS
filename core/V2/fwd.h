//
// Created by erik on 11/22/16.
//

#ifndef SPATACS_CORE_FWD_H
#define SPATACS_CORE_FWD_H

namespace spatacs
{
namespace core
{
namespace ecs
{
    template<class ID, class... Components>
    struct Config;

    template<class... Components>
    class ComponentStorage;

    template<class Config>
    class EntityHandle;

    template<class Config>
    class EntityStorage;

    template<class Signature>
    class System;

    template<class Config>
    class EntityManager;
}
// pull important things out of ecs namespace
using ecs::EntityManager;
using ecs::EntityHandle;
}
}

#endif //SPATACS_CORE_FWD_H
