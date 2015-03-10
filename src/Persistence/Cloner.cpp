#include "Cloner.hpp"
#include "../Game/GameManager.hpp"
#include <queue>

namespace Dungeon {

	ObjectPointer Cloner::getShallowClone() {
		Base* cloned = orig->createObject();
		// Add new Id
		string newId = orig->getObjectType() + "/" + RANDID;
		cloned->setId(newId);
		// What if beforeLoad did something with relations...
		orig->getGameManager()->insertObject(cloned);

		orig->beforeStore(*this);
		orig->registerProperties(*this);
		orig->afterStore(*this);

		storing = true;

		cloned->beforeLoad(*this);
		cloned->registerProperties(*this);
		cloned->afterLoad(*this);
		cloned->save();
		
		LOGS("Cloner", Debug) << "Created a copy with id " << newId << "." << LOGF;

		return cloned;
	}

	ObjectPointer Cloner::getDeepClone() {
		ObjectPointer clone = this->getShallowClone();
		GameManager* gm = orig->getGameManager();
		// Copy all relations
		try {
			for (auto& relList : orig->getRelations(Relation::Master)) {
				if (!relList.second.empty()) {
					for (auto& rel : relList.second) {
						gm->createRelation(clone, rel.second, relList.first);
					}
				}
			}
		} catch (const std::out_of_range& e) {

		}
		try {
			for (auto& relList : orig->getRelations(Relation::Slave)) {
				if (!relList.second.empty()) {
					for (auto& rel : relList.second) {
						gm->createRelation(rel.second, clone, relList.first);
					}
				}
			}
		} catch (const std::out_of_range& e) {

		}
		return clone;
	}

	ObjectPointer Cloner::shallowClone(ObjectPointer original) {
		Cloner cl = Cloner(original);
		LOGS("Cloner", Debug) << "Making shallow clone of object " << original.getId() << "." << LOGF;
		return cl.getShallowClone();
	}

	ObjectPointer Cloner::deepClone(ObjectPointer original) {
		Cloner cl = Cloner(original);
		LOGS("Cloner", Debug) << "Making deep clone of object " << original.getId() << "." << LOGF;
		return cl.getDeepClone();
	}

	IPropertyStorage& Cloner::have(bool& prop, string id, string desc, bool editable) {
		if (storing) {
			prop = *((bool*) properties.front());
			properties.pop();
		} else {
			properties.push((void*) &prop);
		}
		return *this;
	}

	IPropertyStorage& Cloner::have(int& prop, string id, string desc, bool editable) {
		if (storing) {
			prop = *((int*) properties.front());
			properties.pop();
		} else {
			properties.push((void*) &prop);
		}
		return *this;
	}

	IPropertyStorage& Cloner::have(long& prop, string id, string desc, bool editable) {
		if (storing) {
			prop = *((long*) properties.front());
			properties.pop();
		} else {
			properties.push((void*) &prop);
		}
		return *this;
	}

	IPropertyStorage& Cloner::have(string& prop, string id, string desc, bool editable) {
		if (storing) {
			prop.assign(*((string*) properties.front()));
			properties.pop();
		} else {
			properties.push((void*) &prop);
		}
		return *this;
	}
}