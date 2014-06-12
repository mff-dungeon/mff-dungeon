#include "AddIObject.hpp"

namespace Dungeon {
	
	AddIObject::AddIObject(string className, const IObject* o) {
		ObjectList::getInstance().addIObject(className, o);
	}
}