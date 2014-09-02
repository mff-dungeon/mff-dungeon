#include "common.hpp"

#ifndef OBJECTPOINTER_HPP
#define	OBJECTPOINTER_HPP

namespace Dungeon {

    struct ObjectPointer {
    public:
        ObjectPointer() : gm(NULL), id("") {} // "Null OP"
        ObjectPointer(GameManager *gm, objId id)  : gm(gm), id(id) {}
        ObjectPointer(const ObjectPointer& other) : gm(other.gm), id(other.id) {}

        virtual ~ObjectPointer() {}

        IObject *get() const;
        
        objId getId() const
        {
            return this->id;
        }
        
        bool isLoaded() const;

        /**
         * You can use it either as an IObject*...
         */
        operator IObject* () const {
            return get();
        }
        
        /**
         * ... or as id ...
         */
        operator const char * () {
            return getId().c_str();
        }
        
        /**
         * .. or as a pointer.
         */
        IObject* operator->() const {
            return get();
        }
        
        IObject* operator*() const {
            return get();
        }
        
        bool operator!() const {
            return gm == NULL;
        }
        
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
