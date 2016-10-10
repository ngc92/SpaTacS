//
// Created by erik on 10/7/16.
//

#ifndef SPATACS_IRRRAII_H
#define SPATACS_IRRRAII_H

#include <memory>
#include <irrlicht/IReferenceCounted.h>

namespace spatacs {
    namespace ui {
        struct dropper
        {
            /// Default constructor
            constexpr dropper() noexcept = default;

            /// drop on call.
            void operator()(irr::IReferenceCounted* ptr) const
            {
                ptr->drop();
            }
        };

        template<class T>
        using drop_ptr = std::unique_ptr<T, dropper>;
    }
}

#endif //SPATACS_IRRRAII_H
