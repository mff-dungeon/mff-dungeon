#ifndef OBJECTLOADER_HPP
#define	OBJECTLOADER_HPP

#include "../common.hpp"
#include "DatabaseHandler.hpp"

using namespace std;

namespace Dungeon {

	/**
	 * Load & store objects to some storage. 
	 * Implemented using a SQLite database, can be inherited.
	 */
	class ObjectLoader {
	public:
		Base* loadObject(const objId& oid);
		void saveObject(Base *obj);
		vector<objId> getObjectList();
	};
}

#endif	/* OBJECTLOADER_HPP */

