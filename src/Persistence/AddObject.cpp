#include "AddObject.hpp"
#include "ObjectList.hpp"

namespace Dungeon {

	AddObject::AddObject(string className, const Base* o) {
		ObjectList::getInstance().addIObject(className, o);
	}
}