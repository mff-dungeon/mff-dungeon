#include <sqlite3.h>
#include <string>
#include <iostream>

#include "ObjectLoader.hpp"
#include "Archiver.hpp"
#include "../Objects/Virtual/Base.hpp"

namespace Dungeon {

	Base* ObjectLoader::loadObject(const objId& oid) {
		stringstream cDataStream;
		string cName;
		int err = DatabaseHandler::getInstance().loadObject(oid, cName, cDataStream);
		if (err != 0) {
			LOGS(Warning) << "Loading object with id " << oid << " failed." << LOGF;
			return nullptr;
		}
		Archiver as(&cDataStream);
		Base* loaded = Base::load(as, cName);
		loaded->setId(oid);
		LOGS(Debug) << "Loaded object with id " << oid << "." << LOGF;
		return loaded;
	}

	void ObjectLoader::saveObject(Base* object) {
		stringstream s;
		Archiver as(&s);

		string id, cName;
		object->store(as, id, cName);

		stringstream *cDataStream = new stringstream();
		*cDataStream << as.printStream();
		string cData = cDataStream->str();

		int err = DatabaseHandler::getInstance().saveObject(id, cName, cData);
		delete cDataStream;
		if (err != DatabaseHandler::E_OK) {
			LOGS(Error) << "Saving object with " << id << " failed." << LOGF;
		}
		LOGS(Debug) << "Saved object with id " << id << "." << LOGF;
	}

	vector<objId> ObjectLoader::getObjectList() {
		vector<objId> list;
		DatabaseHandler::getInstance().getObjectList(list);
		return list;
	}

}