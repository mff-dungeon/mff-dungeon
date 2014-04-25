#include "GameManager.hpp"

namespace Dungeon {

	GameManager::GameManager() {
		loader = new ObjectLoader();
	}

	IObject* GameManager::getObject(objId id) {
		IObject * r;
		r = this->objects.find(id);
		if (r == 0) {
			r = loader->loadObject(id);
			this->objects.insert(r);
		}
		return r;
	}

	ObjectPointer* GameManager::getObjectPointer(objId id) {
		return new ObjectPointer(this, id);
	}

	void GameManager::insertObject(IObject* obj) {
		objects.insert(obj);
		loader->saveObject(obj);
	}
}
