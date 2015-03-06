#include "ObjectList.hpp"

namespace Dungeon {

	ObjectList::~ObjectList() { // Destructor - clean the list!
		for (map<string, const Base*>::const_iterator it = names.begin(); it != names.end(); it++) {
			delete it->second;
		}
		names.clear();
	}

	ObjectList& ObjectList::getInstance() { // Singleton class
		static ObjectList instance;
		return instance;
	}

	// Register the object into the class

	void ObjectList::addIObject(string className, const Base* o) {
		map<string, const Base*>::const_iterator it = names.find(className);
		if (it == names.end()) {
			names.insert(pair<string, const Base*>(className,o));
			LOGS("ObjectList", Debug) << "Registered class " << className << " into an object list." << LOGF;
		}
	}

	// Method copying the object in the map by creating a new instance

	Base* ObjectList::create(string className) {
		map<string, const Base*>::const_iterator it = names.find(className);
		if (it == names.end()) {
			LOGS("ObjectList", Error) << "Attempted to create an instance of unknown class: " << className << "." << LOGF;
			return nullptr;
		}
		LOGS("ObjectList", Debug) << "Created a new instance of " << className << LOGF;
		return it->second->createObject();
	}
}