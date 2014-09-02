#include "ObjectPointer.hpp"
#include "GameManager.hpp"

namespace Dungeon
{

	bool ObjectPointer::isLoaded() const {
		return gm->hasObjectLoaded(id);
	}
	
	IObject* ObjectPointer::get() const {
		return gm->getObject(this->id);
	}

}
