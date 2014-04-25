#include "ObjectLoader.hpp"
#include "Objects/Dummy.hpp"

namespace Dungeon {
	
	IObject *ObjectLoader::loadObject(objId id) {
		return new DummyObject();
	}
	
	void ObjectLoader::saveObject(IObject *obj) {
	}
}