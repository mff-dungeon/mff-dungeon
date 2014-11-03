#include "ObjectList.hpp"

namespace Dungeon {

	ObjectList::~ObjectList() { // Desctructor - clean the list!
		for (map<string, const Base*>::const_iterator it = names.begin(); it != names.end(); it++) {
			const Base* o = it->second;
			delete o;
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
		}
	}

	// Method copying the object in the map by creating a new instance

	Base* ObjectList::create(string className) {
		map<string, const Base*>::const_iterator it = names.find(className);
		if (it == names.end()) return 0;
		const Base* o = it->second;
		return o->createObject();
	}
}