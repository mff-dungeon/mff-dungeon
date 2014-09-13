#include "DoorLock.hpp"
#include "../Objects/Alive.hpp"
#include "../Objects/Inventory.hpp"

namespace Dungeon {

	DoorLock::DoorLock() {

	}
	
	DoorLock::DoorLock(objId id) : Trap(id) {

	}

	DoorLock::~DoorLock() {

	}

	ObjectPointer DoorLock::getKey() const {
		return this->getSingleRelation("keytemplate", Relation::Master);
	}

	DoorLock* DoorLock::setKey(ObjectPointer key) {
		this->setSingleRelation("keytemplate", key, Relation::Master);
		return this;
	}

	bool DoorLock::consumesKey() const {
		return consumeKey;
	}

	DoorLock* DoorLock::setConsumesKey(bool consumesKey) {
		this->consumeKey = consumesKey;
		return this;
	}

	void DoorLock::trigger(string event, ObjectPointer target, ActionDescriptor* ad) {
		if(!ad->getAlive()->hasItemType(getKey()->getObjectType())) {
			throw TrapException(this);
		}
		if(consumesKey()) {
			ObjectPointer backpack = ad->getAlive()->getBackpack();
			backpack.assertExists("Where do you keep your key?")
					.assertType<Inventory>("You have equipped a non-backpack. How? ");
			const ObjectMap& items = backpack.unsafeCast<Inventory>()->getRelations(Relation::Master, R_INVENTORY);
			for(auto& item : items) {
				if(item.second->getObjectType() == getKey()->getObjectType()) {
					backpack.unsafeCast<Inventory>()->removeItem(item.second);
					break;
				}
			}
			*ad << "As you use your " << getKey().safeCast<IDescriptable>()->getName()
				<< " it disappears. ";
		}
		else {
			*ad << "You use your " << getKey().safeCast<IDescriptable>()->getName() << ". ";
		}
	}

	void DoorLock::exceptionTrigger(ActionDescriptor* ad) {
		*ad << "You need to have a " << getKey().safeCast<IDescriptable>()->getName() << " to pass this door. ";
		ad->setAction(0);
	}
	
	void DoorLock::registerProperties(IPropertyStorage& storage) {
		storage.have(consumeKey, "doorlock-consumekey", "True when unlocking the door removes the key. ")
				.have(keyId, "doorlock-keyid", "Item type of the required key");
		Trap::registerProperties(storage);
	}

	PERSISTENT_IMPLEMENTATION(DoorLock)
}

