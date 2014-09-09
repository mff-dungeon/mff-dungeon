#include <queue>

#include "Cloner.hpp"

namespace Dungeon {
	
	ObjectPointer Cloner::getClone() {
		IObject* cloned = orig->createObject();
		
		orig->beforeStore(*this);
		orig->registerProperties(*this);
		orig->afterStore(*this);
		
		storing = true;
		
		cloned->beforeLoad(*this);
		cloned->registerProperties(*this);
		cloned->afterLoad(*this);
		
		gm->insertObject(cloned);
		return cloned;
	}

	ObjectPointer Cloner::clone(ObjectPointer original, GameManager* gm) {
		Cloner cl = Cloner(original, gm);
		return cl.getClone();
	}


	IPropertyStorage& Cloner::have(bool& prop, string id, string desc, bool editable) {
		if(storing) {
			prop = *((bool*) properties.front());
			properties.pop();
		}
		else {
			properties.push((void*) &prop);
		}
		return *this;
	}

	IPropertyStorage& Cloner::have(int& prop, string id, string desc, bool editable) {
		if(storing) {
			prop = *((int*) properties.front());
			properties.pop();
		}
		else {
			properties.push((void*) &prop);
		}
		return *this;
	}

	IPropertyStorage& Cloner::have(string& prop, string id, string desc, bool editable) {
		if(storing) {
			prop.assign(*((string*) properties.front()));
			properties.pop();
		}
		else {
			properties.push((void*) &prop);
		}
		return *this;
	}
}