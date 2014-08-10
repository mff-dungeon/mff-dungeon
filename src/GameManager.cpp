#include <c++/4.9.1/stdexcept>

#include "GameManager.hpp"

namespace Dungeon {

	GameManager::GameManager() {
        LOG("GameManager") << "Created." << LOGF;
		loader = new ObjectLoader();
		aqueue = new ActionQueue(this);
	}

	IObject* GameManager::getObject(objId id) {
		IObject * r;
		r = this->objects.find(id);
		if (r == 0) {
			r = loader->loadObject(id);
			if (r != 0)
				this->objects.insert(r);
			else
				throw new runtime_error("No such object found");
		}
        LOG("GameManager") << "Looking up object '" << id << "'." << LOGF;
		return r;
	}

	ObjectPointer* GameManager::getObjectPointer(objId id) {
		return new ObjectPointer(this, id);
	}

	void GameManager::insertObject(IObject* obj) {
		objects.insert(obj);
		loader->saveObject(obj);
	}
	
	ActionQueue* GameManager::getQueue() {
		return this->aqueue;
	}
	
	void GameManager::shutdown() {
		this->aqueue->stop();
	}
}
