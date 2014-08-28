#ifndef OBJECTLOADER_HPP
#define	OBJECTLOADER_HPP

#include "common.hpp"
#include "DatabaseHandler.hpp"

using namespace std;

namespace Dungeon {

    /**
     * Load & store objects to some storage. 
     * By default implemented database, can be inherited.
     */
    class ObjectLoader {
    public:
        IObject* loadObject(objId oid);
        void saveObject(IObject *obj);
    };
}

#endif	/* OBJECTLOADER_HPP */

