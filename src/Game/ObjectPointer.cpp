#include "ObjectPointer.hpp"
#include "../exceptions.hpp"
#include "GameManager.hpp"

namespace Dungeon
{

	bool ObjectPointer::isLoaded() const {
		return this->id == "" || gm->hasObjectLoaded(id);
	}
	
	Base* ObjectPointer::get() const {
		if (!gm) return nullptr;
		return gm->getObjectInstance(this->id);
	}

	const ObjectPointer& ObjectPointer::assertExists(string msg) const {
		if (!gm || !gm->hasObject(id))
			throw ObjectLost(msg + "id: " + id);
		return *this;
	}
	
	const ObjectPointer& ObjectPointer::assertRelation(string type, ObjectPointer other, Relation::Dir master, string msg) const {
		if (isLoaded()) {
			if (!get()->hasRelation(type, other, master))
				throw ObjectLost(msg);
		} else {
			bool found = (master == Relation::Master) 
					? gm->hasRelation(*this, other, type)
					: gm->hasRelation(other, *this, type);
			if(!found)
				throw GameStateChanged(msg);
		}
		return *this;
	}


	
	

}
