#include "ObjectGroup.hpp"
#include "Objects/IDescriptable.hpp"

namespace Dungeon {

	ObjectGroup::ObjectGroup(IObject *obj) : ObjectGroupMap() {
		this->insert(getPair(obj));
	}

	ObjectGroup::ObjectGroup(ObjectPointer ptr) : ObjectGroupMap() {
		this->insert(getPair(ptr));
	}

	ObjectGroup::ObjectGroup(const vector<IObject *>& objects) : ObjectGroupMap() {
		for (IObject *obj : objects) {
			this->insert(getPair(obj));
		}
	}

	ObjectGroup::ObjectGroup(const vector<ObjectPointer>& pointers) : ObjectGroupMap() {
		for (ObjectPointer ptr : pointers) {
			this->insert(getPair(ptr));
		}
	}

	ObjectGroup::ObjectGroup(ObjectMap map) : ObjectGroupMap(map.begin(), map.end()) {
 }

	ObjectGroupMap::value_type ObjectGroup::getPair(IObject *obj) {
		return ObjectGroupMap::value_type(obj->getId(), obj);
	}

	ObjectGroupMap::value_type ObjectGroup::getPair(ObjectPointer ptr) {
		return ObjectGroupMap::value_type(ptr.getId(), ptr);
	}

	string ObjectGroup::explore() {
		if (this->size() == 0) return "";
		string sentence = "";
		ObjectGroup::iterator it;
		string objectType = objId_getType(this->begin()->first);
		vector<ObjectPointer> sameTypeObjects;

		for (it = this->begin(); it != this->end(); ) {
			if (!it->second->instanceOf(IDescriptable)) {
				sentence += "There is an object (" + it->second.getId() + "). ";
				continue;
			}
			objectType = objId_getType(it->first);
			
			while (it != this->end() && objectType == objId_getType(it->first)) {
				sameTypeObjects.push_back(it->second);
				it++;
			}
			
			if (sameTypeObjects.empty()) {
				continue;
			} else if (sameTypeObjects.size() == 1) {
				sentence += sameTypeObjects.front().unsafeCast<IDescriptable>()->getDescriptionSentence() + " ";
			} else {
				sentence += sameTypeObjects.front().unsafeCast<IDescriptable>()->getGroupDescriptionSentence(sameTypeObjects) + " ";
			}
			sameTypeObjects.clear();
		}
		
		return sentence;
	}


}
