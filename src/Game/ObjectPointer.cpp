#include "ObjectPointer.hpp"
#include "../exceptions.hpp"
#include "GameManager.hpp"

namespace Dungeon
{

	bool ObjectPointer::isLoaded() const {
		return !!strong_obj || !weak_obj.expired() || gm->hasObjectLoaded(id);
	}
	
	ObjectPointer::ptr_t ObjectPointer::get() const {
		if (!!strong_obj) return strong_obj;
		
		// Intentionally not storing the locked ptr to strong_obj
		ptr_t lptr = weak_obj.lock();
		if (!!lptr) return lptr;
		
		if (gm) {
			lptr = gm->getObjectInstance(this->id);
			const_cast<ObjectPointer*>(this)->weak_obj = lptr;
		}
		return lptr;
	}

	const ObjectPointer& ObjectPointer::assertExists(string msg) const {
		if (!strong_obj && weak_obj.expired() && (!gm || !gm->hasObject(id)))
			throw ObjectLost(msg + "id: " + id);
		return *this;
	}
	
	const ObjectPointer& ObjectPointer::assertRelation(const string& type, ObjectPointer other, Relation::Dir master, string msg) const {
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
