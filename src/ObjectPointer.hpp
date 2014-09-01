#include "common.hpp"

#ifndef OBJECTPOINTER_HPP
#define	OBJECTPOINTER_HPP

#include "GameManager.hpp"

namespace Dungeon {

    struct ObjectPointer {
    public:
        ObjectPointer() {} // Needed for std functions, otherwise unusable
        ObjectPointer(GameManager *gm, objId id);
        ObjectPointer(const ObjectPointer& other) : gm(other.gm), id(other.id) {}

        virtual ~ObjectPointer() {}

        IObject *get();
        objId getId();
        
        bool isLoaded();

        bool operator==(const ObjectPointer& other) const {
            return other.id == id;
        }
        
        bool operator!=(const ObjectPointer& other) const {
            return other.id != id;
        }

    private:
        GameManager *gm;
        objId id;
    };
}

#endif
