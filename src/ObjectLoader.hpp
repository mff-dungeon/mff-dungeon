#ifndef OBJECTLOADER_HPP
#define	OBJECTLOADER_HPP

#include "common.hpp"
#include "DatabaseHandler.hpp"

using namespace std;

namespace Dungeon {

    class ObjectLoader {
    public:
        IObject* loadObject(objId oid);
        void saveObject(IObject *obj);
    };
}

#endif	/* OBJECTLOADER_HPP */

