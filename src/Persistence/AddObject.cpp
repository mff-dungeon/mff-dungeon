#include "AddObject.hpp"
#include "ObjectList.hpp"

namespace Dungeon {

	AddObject::AddObject(const string& className, const Base* o) {
		ObjectList::getInstance().addIObject(className, o);
	}
}