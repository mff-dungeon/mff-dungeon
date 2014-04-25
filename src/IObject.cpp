#include "IObject.hpp"

namespace Dungeon {

	objId IObject::getId() {
		return id;
	}

	IObject * IObject::setId(objId id) {
		this->id = id;
		return this;
	}
}