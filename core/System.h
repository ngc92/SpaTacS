//
// Created by erik on 10/8/16.
//

#ifndef SPATACS_SYSTEM_H
#define SPATACS_SYSTEM_H

#include <boost/mpl/vector.hpp>
#include <boost/mpl/for_each.hpp>
#include <tuple>

namespace spatacs
{
    namespace core
    {
        template<class... Components>
        struct Signature
        {
            using type_list = boost::mpl::vector<Components...>;
        };

        template<class Entity, class Imp, class... Sigs>
        class System
        {
            using bits_t     = typename Entity::bits_t;
            using signatures = std::tuple<Sigs...>;
        public:
            System() { }

            void operator()(Entity& entity)
            {
                try_apply<0>(entity);
            }
        private:
            Imp& imp() { return static_cast<Imp&>(*this); }
            const Imp& imp() const { return static_cast<const Imp&>(*this); }

            template<std::size_t I>
            void try_apply(Entity& entity, std::enable_if_t<I != sizeof...(Sigs), void*> v = nullptr)
            {
                using sig = std::tuple_element_t<I, signatures>;
                if(match<sig>( entity.bits() ))
                {
                    apply_matching(entity, sig());
                } else
                {
                    try_apply<I+1>(entity);
                }
            }

            // end of the recursion, we need ot get here by the enable_if overload
            template<std::size_t I>
            void try_apply(Entity& entity, std::enable_if_t<I == sizeof...(Sigs), void*> v = nullptr)
            {
            }

            template<class... SigTypes>
            void apply_matching(Entity& entity, Signature<SigTypes...>&& s)
            {
                imp().apply( entity.template get<SigTypes>()... );
            }

            // overload in case the first thing in the signature is the entity.
            /// \todo maybe we can generalize this a bit later on.
            template<class... SigTypes>
            void apply_matching(Entity& entity, Signature<Entity&, SigTypes...>&& s)
            {
                imp().apply( entity, entity.template get<SigTypes>()... );
            }

            // matcher
            template<class Sig>
            constexpr static bool match(const bits_t& bits)
            {
                bits_t reference;
                boost::mpl::for_each<typename Sig::type_list>([&](auto u){

                    // this is correct, but does not work in gcc 5
                    // constexpr auto idx = Entity::template index_v<decltype(u)>;
                    constexpr auto idx = Entity::template index<decltype(u)>();
                    reference.set(idx);
                });

                return (bits & reference) == reference;
            }
        };

        template<class Entity, class... Components, class Lambda>
        auto make_system(Lambda&& l)
        {
            class Sys : public System<Entity, Sys, Signature<Components...>>
            {
            public:
                Sys(Lambda func) : mLambda(std::move(func)) { }
                void apply(Components&... comps)
                {
                    mLambda(comps...);
                }
            private:
                Lambda mLambda;
            };

            return Sys(std::move(l));
        };
    }
}

#endif //SPATACS_SYSTEM_H