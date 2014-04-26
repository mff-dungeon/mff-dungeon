#ifndef OBJECTLOADER_HPP
#define	OBJECTLOADER_HPP

#include <sqlite3.h>
#include "common.hpp"

using namespace std;

namespace Dungeon {

    class ObjectLoader {
    public:
        IObject *loadObject(objId id);
        void saveObject(IObject *obj);
	
	private:
		sqlite3* dbConnection;
		sqlite3_stmt* dbStatement;
		
		std::string loadFromDb(objId id);
    };
}

#endif	/* OBJECTLOADER_HPP */

