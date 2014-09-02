#include <sqlite3.h>
#include <string>
#include <iostream>

#include "ObjectLoader.hpp"
#include "Objects/Dummy.hpp"

namespace Dungeon {

	IObject* ObjectLoader::loadObject(objId oid) {
		stringstream cDataStream;
		string cName;
		int err = DatabaseHandler::getInstance().loadObject(oid, cName, cDataStream);
		if(err != 0) {
			LOG("Loader") << "Error loading object, id " << oid << ". Error code: " << err << LOGF;
			return 0;
		}
		Archiver as(&cDataStream);
		IObject* loaded = IObject::load(as, cName);
		loaded->setId(oid);
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

		int err = DatabaseHandler::getInstance().saveObject(id, cName, cData);
		delete cDataStream;
		if(err != DatabaseHandler::E_OK) {
			LOGS("Loader", Fatal) << "Error saving object " << id << ", error code " << err << LOGF;
		}
	}
	
	vector<objId> ObjectLoader::getObjectList() {
		vector<objId> list;
		DatabaseHandler::getInstance().getObjectList(list);
		return list;
	}

}