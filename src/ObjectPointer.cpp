#include "ObjectPointer.hpp"
#include "exceptions.hpp"
#include "GameManager.hpp"

namespace Dungeon
{

	bool ObjectPointer::isLoaded() const {
		return gm->hasObjectLoaded(id);
	}
	
	IObject* ObjectPointer::get() const {
		if (!gm) return nullptr;
		return gm->getObjectInstance(this->id);
	}

	const ObjectPointer& ObjectPointer::assertExists(string msg) const {
		if (!gm->hasObject(id))
			throw ObjectLost(msg);
		return *this;
	}
	
	const ObjectPointer& ObjectPointer::assertRelation(string type, ObjectPointer other, Relation::Dir master, string msg) const {
		/* TODO gm has no such method :)
		if (isLoaded()) {
			if (!get()->hasRelation(type, other, master))
				throw ObjectLost(msg);
		} else {
			gm->hasRelation(...)
		}*/
		if (!get()->hasRelation(type, other, master))
			throw GameStateChanged(msg);
		return *this;
	}


	
	

}
