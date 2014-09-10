#include <queue>

#include "Cloner.hpp"

namespace Dungeon {
	
	ObjectPointer Cloner::getShallowClone() {
		IObject* cloned = orig->createObject();
		// Add new Id
		string newId = objId_getType(orig.getId());
		if(newId.find("template" == 0)) newId = newId.substr(9);
		newId += "/" + RANDID;
		cloned->setId(newId);
		// What if beforeLoad did something with relations...
		gm->insertObject(cloned);
		
		orig->beforeStore(*this);
		orig->registerProperties(*this);
		orig->afterStore(*this);
		
		storing = true;
		
		cloned->beforeLoad(*this);
		cloned->registerProperties(*this);
		cloned->afterLoad(*this);
		cloned->save();
		
		return cloned;
	}
		
	ObjectPointer Cloner::getDeepClone() {
		ObjectPointer clone = this->getShallowClone();
		// Copy all relations
		clone->relation_master.insert(orig->relation_master.begin(), orig->relation_master.end());
		clone->relation_slave.insert(orig->relation_slave.begin(), orig->relation_slave.end());
		return clone;
	}


	ObjectPointer Cloner::shallowClone(ObjectPointer original) {
		Cloner cl = Cloner(original);
		return cl.getShallowClone();
	}

	ObjectPointer Cloner::deepClone(ObjectPointer original) {
		Cloner cl = Cloner(original);
		return cl.getDeepClone();
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