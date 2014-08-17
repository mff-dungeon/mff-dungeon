#ifndef OBJECTPOINTER_HPP
#define	OBJECTPOINTER_HPP

#include "common.hpp"
#include "GameManager.hpp"

namespace Dungeon {

    class ObjectPointer {
    public:
        ObjectPointer(GameManager *gm, objId id);
        IObject *get();
        objId getId();
        
        ObjectPointer* clone() {
            return new ObjectPointer(gm, id);
        }

    private:
        GameManager *gm;
        objId id;
    };
}

#endif
