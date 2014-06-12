#include "ObjectList.hpp"

namespace Dungeon {
	
	ObjectList::~ObjectList() {	// Desctructor - clean the list!
		for(map<string, const IObject*>::const_iterator it = names.begin(); it != names.end(); it++) {
			const IObject* o = it->second;
			delete o;
		}
		names.clear();
	}

	ObjectList& ObjectList::getInstance() {		// Singleton class
		static ObjectList instance;
		return instance;
	}

	// Register the object into the class
	void ObjectList::addIObject(string className, const IObject* o) {
		map<string, const IObject*>::const_iterator it = names.find(className);
		if(it == names.end()) {
			names[className] = o;
		}
	}

	// Method copying the object in the map by creating a new instance
	IObject* ObjectList::create(string className) {
		map<string, const IObject*>::const_iterator it = names.find(className);
		if(it == names.end()) return 0;
		const IObject* o = it->second;
		return o->createObject();
	}	
}