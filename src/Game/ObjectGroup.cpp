#include "ObjectGroup.hpp"
#include "../Objects/Virtual/IDescriptable.hpp"
#include "../Objects/Traps/MTATrap.hpp"
#include "../Objects/Human.hpp"

namespace Dungeon {

	ObjectGroup::ObjectGroup() : ObjectGroupMap() { }

	ObjectGroup::ObjectGroup(ObjectPointer ptr) : ObjectGroup() {
		this->insert(getPair(ptr));
	}

	ObjectGroup::ObjectGroup(const vector<ObjectPointer>& pointers) : ObjectGroup() {
		for (ObjectPointer ptr : pointers) {
			this->insert(getPair(ptr));
		}
	}

	ObjectGroup::ObjectGroup(ObjectMap map) : ObjectGroupMap(map.begin(), map.end()) { }

	ObjectGroupMap::iterator ObjectGroup::insertObject(ObjectPointer ptr) {
		return ObjectGroupMap::insert(getPair(ptr));
	}

	ObjectGroupMap::value_type ObjectGroup::getPair(Base *obj) {
		return ObjectGroupMap::value_type(obj->getId(), obj);
	}

	ObjectGroupMap::value_type ObjectGroup::getPair(ObjectPointer ptr) {
		return ObjectGroupMap::value_type(ptr.getId(), ptr);
	}

	void ObjectGroup::explore(ActionDescriptor *ad, bool ignoreOfflineUsers) {
		if (this->size() == 0) return;
		Human* beholder = ad ? ad->getCaller() : nullptr;
		ObjectGroup::iterator it;
		string objectType = objId_getType(this->begin()->first);
		vector<ObjectPointer> sameTypeObjects;

		for (it = this->begin(); it != this->end();) {
			if (!it->second->instanceOf(IDescriptable)) {
				*ad << "There is an object (" + it->second.getId() + ")." << eos;
				it++;
				continue;
			} else if (it->second->instanceOf(Human) && it->second.unsafeCast<Human>()->getPresence() == Human::Presence::Offline) {
				// These aren't the Alives we're looking for.
				it++;
				continue;
			}
			objectType = objId_getType(it->first);

			while (it != this->end() && objectType == objId_getType(it->first)) {
				sameTypeObjects.push_back(it->second);
				it++;
			}

			if (beholder) {
				for (auto ptr = sameTypeObjects.begin(); ptr != sameTypeObjects.end(); ptr++) {
					beholder->see(*ptr);
				}
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
	
	ObjectPointer ObjectGroup::match(string name) {
		FuzzyStringMatcher<ObjectPointer> matcher;
		ObjectGroup::iterator it;
		for (it = this->begin(); it != this->end(); it++) {
			IDescriptable* obj = it->second.safeCast<IDescriptable>();
			if (obj) {
				matcher.add(obj->getLongName(), it->second);
				matcher.add(obj->getName(), it->second);
			}
		}

		return matcher.find(name);
	}
	
}
