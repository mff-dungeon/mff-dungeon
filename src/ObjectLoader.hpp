#ifndef OBJECTLOADER_HPP
#define	OBJECTLOADER_HPP

#include <iostream>
#include <fstream>
#include "common.hpp"

using namespace std;

namespace Dungeon {

    class ObjectLoader {
    public:
        IObject *loadObject(objId id);
        void saveObject(IObject *obj);
    };
}

#endif	/* OBJECTLOADER_HPP */

