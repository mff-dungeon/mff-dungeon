#include <sqlite3.h>
#include <string>
#include <iostream>

#include "ObjectLoader.hpp"
#include "Objects/Dummy.hpp"

namespace Dungeon {

	IObject* ObjectLoader::loadObject(objId oid) {
		stringstream cDataStream;
		string cName;
		if(DatabaseHandler::getInstance().loadObject(oid, cName, cDataStream) != 0)
			return 0;
		Archiver as(&cDataStream);
		IObject* loaded = IObject::load(as, cName);
		return loaded;
	}

	void ObjectLoader::saveObject(IObject* object) {
		stringstream s;
		Archiver as(&s);

		string id, cName;
		object->store(as, id, cName);

		stringstream *cDataStream = new stringstream();
		*cDataStream << as.printStream();
		string cData = cDataStream->str();

		DatabaseHandler::getInstance().saveObject(id, cName, cData);
		delete cDataStream;
	}
}