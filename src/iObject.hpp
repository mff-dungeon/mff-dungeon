#ifndef IOBJECT_HPP
#define	IOBJECT_HPP

#include "common.hpp"

namespace Dungeon {
    class IObject {
    public:
        virtual objId getId() = 0;
    };
}

#endif

