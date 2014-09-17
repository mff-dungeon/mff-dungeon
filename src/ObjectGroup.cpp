#include "ObjectGroup.hpp"
#include "Objects/IDescriptable.hpp"
#include "Traps/MTATrap.hpp"

namespace Dungeon {

	ObjectGroup::ObjectGroup() : ObjectGroupMap() { }


	ObjectGroup::ObjectGroup(IObject *obj) : ObjectGroup() {
		this->insert(getPair(obj));
	}

	ObjectGroup::ObjectGroup(ObjectPointer ptr) : ObjectGroup() {
		this->insert(getPair(ptr));
	}

	ObjectGroup::ObjectGroup(const vector<IObject *>& objects) : ObjectGroup() {
		for (IObject *obj : objects) {
			this->insert(getPair(obj));
		}
	}

	ObjectGroup::ObjectGroup(const vector<ObjectPointer>& pointers) : ObjectGroup() {
		for (ObjectPointer ptr : pointers) {
			this->insert(getPair(ptr));
		}
	}

	ObjectGroup::ObjectGroup(ObjectMap map) : ObjectGroupMap(map.begin(), map.end()) {
	}
	
	ObjectGroupMap::iterator ObjectGroup::insertObject(ObjectPointer ptr) {
		return ObjectGroupMap::insert(getPair(ptr));
	}

	ObjectGroupMap::value_type ObjectGroup::getPair(IObject *obj) {
		return ObjectGroupMap::value_type(obj->getId(), obj);
	}

	ObjectGroupMap::value_type ObjectGroup::getPair(ObjectPointer ptr) {
		return ObjectGroupMap::value_type(ptr.getId(), ptr);
	}
        
	void ObjectGroup::explore(ActionDescriptor *ad) {
		if (this->size() == 0) return;
		ObjectGroup::iterator it;
		string objectType = objId_getType(this->begin()->first);
		vector<ObjectPointer> sameTypeObjects;

		for (it = this->begin(); it != this->end(); ) {
				if (!it->second->instanceOf(IDescriptable)) {
						*ad << "There is an object (" + it->second.getId() + ")." << eos;
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
						*ad << sameTypeObjects.front().unsafeCast<IDescriptable>()->getDescriptionSentence() << eos;
				} else {
						*ad << sameTypeObjects.front().unsafeCast<IDescriptable>()->getGroupDescriptionSentence(sameTypeObjects) << eos;
				}
				sameTypeObjects.clear();
		}
	}
}
