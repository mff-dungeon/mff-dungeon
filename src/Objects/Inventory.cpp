#include "Inventory.hpp"
#include "Alive.hpp"
#include "../Utils/RandomString.hpp"
#include "../Utils/SentenceJoiner.hpp"
#include "../Utils/RegexMatcher.hpp"
#include "../Game/GameManager.hpp"
#include "../Persistence/Cloner.hpp"
#include "Resource.hpp"
#include "Location.hpp"
#include <unistd.h>

namespace Dungeon {

	Inventory::Inventory() {
		this->maxSpace = DEFAULT_SPACE;
		this->maxWeight = DEFAULT_WEIGHT;
		this->usedSpace = 0;
		this->usedWeight = 0;
	}

	Inventory::Inventory(const objId& id) : Wearable(id) {
		this->maxSpace = DEFAULT_SPACE;
		this->maxWeight = DEFAULT_WEIGHT;
		this->usedSpace = 0;
		this->usedWeight = 0;
	}

	Inventory* Inventory::setMaxSpace(int maxSpace) {
		this->maxSpace = maxSpace;
		return this;
	}

	int Inventory::getMaxSpace() const {
		return this->maxSpace;
	}

	int Inventory::getFreeSpace() const {
		return (this->maxSpace - this->usedSpace);
	}

	Inventory* Inventory::setMaxWeight(int maxWeight) {
		this->maxWeight = maxWeight;
		return this;
	}

	int Inventory::getMaxWeight() const {
		return this->maxWeight;
	}

	int Inventory::getFreeWeight() const {
		return (this->maxWeight - this->usedWeight);
	}

	int Inventory::getBaseSize() const {
		return Item::getSize();
	}

	Inventory* Inventory::setBaseSize(int size) {
		Item::setSize(size);
		return this;
	}

	int Inventory::getSize() const {
		return getBaseSize() + usedSpace;
	}

	int Inventory::getBaseWeight() const {
		return Item::getWeight();
	}

	Inventory* Inventory::setBaseWeight(int weight) {
		Item::setWeight(weight);
		return this;
	}

	int Inventory::getWeight() const {
		return getBaseWeight() + usedWeight;
	}

	bool Inventory::canAdd(ObjectPointer itemPtr) {
		itemPtr.assertType<Item>("You can add only items into your backpack.");
		Item* item = itemPtr.safeCast<Item>();
		return (getFreeWeight() >= item->getWeight() && getFreeSpace() >= item->getSize());
	}

	ObjectPointer Inventory::addItem(ObjectPointer itemPtr) {
		itemPtr.assertType<Item>("You can add only items into your backpack.");
		Item* item = itemPtr.safeCast<Item>();
		LOGS(Debug) << "Adding item " << item->getName() << " to inventory " << getName() << "." << LOGF;
		this->usedWeight += item->getWeight();
		this->usedSpace += item->getSize();
		if(item->instanceOf(Resource)) {
			ObjectPointer currentPtr = getSingleRelation(R_RESOURCE(((Resource*) item)->getType()), Relation::Master);
			if(!!currentPtr) { // We need to merge and return the old one
				Resource* current = currentPtr.unsafeCast<Resource>();
				Resource* newRes = (Resource*) item;
				current->setQuantity(current->getQuantity() + newRes->getQuantity());
				getGameManager()->deleteObject(newRes);
				save();
				return currentPtr;
			}
			else { // set shortcut & continue processing for saving and so...
				setSingleRelation(R_RESOURCE(((Resource*) item)->getType()), itemPtr);
			}
		}
		this->getGameManager()->createRelation(this, item, R_INVENTORY);
		save();
		return itemPtr;
	}
	
	int Inventory::getResourceQuantity(Resource::ResourceType type) {
		ObjectPointer resource = getSingleRelation(R_RESOURCE(type), Relation::Master);
		if (!!resource) {
			return resource.unsafeCast<Resource>()->getQuantity();
		} else {
			return 0;
		}
	}

	bool Inventory::changeResourceQuantity(Resource::ResourceType type, int deltaQuantity) {
		if(deltaQuantity > 0) return addResource(type, deltaQuantity);
		else if(deltaQuantity < 0) return removeResource(type, -deltaQuantity);
		else return true;
	}
	
	bool Inventory::addResource(Resource::ResourceType type, int added) {
		if(added <= 0) return false;
		ObjectPointer templ = getGameManager()->getObject(string("template/resource/") + Resource::ResourceIdentifier[type] + "/1");
		Resource* templPtr = templ.safeCast<Resource>();
		if(getFreeWeight() < templPtr->getWeight()*added || getFreeSpace() < templPtr->getSize()*added) {
			return false;
		}
		ObjectPointer res = Cloner::shallowClone(getGameManager()->getObject(string("template/resource/") + Resource::ResourceIdentifier[type] + "/1"));
		res.unsafeCast<Resource>()->setQuantity(added);
		addItem(res);
		return true;
	}
	
	bool Inventory::removeResource(Resource::ResourceType type, int removed) {
		if(removed <= 0) return false;
		ObjectPointer current = getSingleRelation(R_RESOURCE(type), Relation::Master);
		Resource* r = current.unsafeCast<Resource>();
		if(r->getQuantity() < removed) {
			return false;
		}
		else {
			getGameManager()->deleteObject(removeItem(current, removed));
			return true;
		}
	}
	
	bool Inventory::setResource(Resource::ResourceType type, int newAmount) {
		return changeResourceQuantity(type, newAmount - getResourceQuantity(type));
	}

	ObjectPointer Inventory::removeItem(ObjectPointer itemPtr, int amount) {
		itemPtr.assertType<Item>("You can only have items in your backpack.");
		if(itemPtr->instanceOf(Resource)) {
			Resource* r = itemPtr.unsafeCast<Resource>();
			LOGS(Debug) << "Removing " << amount << " " << r->getName() << " from inventory " << getName() << "." << LOGF; 
			ObjectPointer current = getSingleRelation(R_RESOURCE(r->getType()), Relation::Master);
			if(current != itemPtr) throw GameStateInvalid("Trying to remove a resource from inventory, which is not there");
			if(amount >= r->getQuantity()) {
				this->usedWeight -= r->getWeight();
				this->usedSpace -= r->getSize();
				this->getGameManager()->removeRelation(this, current, R_INVENTORY);
				setSingleRelation(R_RESOURCE(r->getType()), nullptr, Relation::Master);
				save();
				return current;
			}
			else {
				ObjectPointer partPtr = r->split(amount);
				Resource* part = partPtr.unsafeCast<Resource>();
				this->usedWeight -= part->getWeight();
				this->usedSpace -= part->getSize();
				save();
				return partPtr;
			}
		}
		Item* item = itemPtr.unsafeCast<Item>();
		LOGS(Debug) << "Removing " << item->getName() << " from inventory " << getName() << "." << LOGF; 
		this->usedWeight -= item->getWeight();
		this->usedSpace -= item->getSize();
		this->getGameManager()->removeRelation(this, item, R_INVENTORY);
		save();
		return itemPtr;
	}

	bool Inventory::contains(ObjectPointer itemPtr) {
		return hasRelation(R_INVENTORY, itemPtr, Relation::Master);
	}

	string Inventory::getContainingSentence() {
		SentenceJoiner items;

		for (auto& item : getRelations(Relation::Master, R_INVENTORY)) {
			items << item.second;
		}
		return items.getSentence("", "There is %.", "There is %.");
	}

	string Inventory::getDescription() const {
		stringstream ss;
		ss << Wearable::getDescription();
		ss << "It can hold " << Utils::weightStr(getMaxWeight());
		if (usedWeight)
			ss << " and currently holds " << Utils::weightStr(usedWeight) << ".";
		else ss << " and now it's empty.";
		return ss.str();
	}

	void Inventory::getActions(ActionDescriptor* ad) {
		// Drop action, adding only if there is anything to drop
		DropAction* action = new DropAction;
		try {
			const ObjectMap& itemsIn = this->getRelations(Relation::Master, R_INVENTORY);
			for (auto& item : itemsIn) {
				// TODO - R_INVENTORY delegating
				item.second->getActionsRecursive(ad);
				action->addTarget(item.second);
			}
		} catch (const std::out_of_range& e) {

		}
		if (action->getTargets().size() > 0) {
			ad->getActionList().addAction(action);
		} else {
			delete action;
		}
	}

	void Inventory::registerProperties(IPropertyStorage& storage) {
		storage.have(maxSpace, "backpack-maxspace", "Maximum space of backpack").
				have(maxWeight, "backpack-maxweight", "Maximum weight of backpack").
				have(usedSpace, "backpack-curspace", "Space currently used", false).
				have(usedWeight, "backpack-curweight", "Weight currently used", false);
		Wearable::registerProperties(storage);
	}

	PERSISTENT_IMPLEMENTATION(Inventory)

	void DropAction::explain(ActionDescriptor* ad) {
		*ad << "Use 'drop ...' to drop items from your backpack. \n" << eos;
	}

	bool DropAction::match(const string& command, ActionDescriptor* ad) {
		if(captureMatcher.match("drop ([0-9]+ )?(.*)", command)) {
			try {
				amount = stoi(captureMatcher.matches[1]);
			} catch (invalid_argument& e) {}
			selectBestTarget(captureMatcher.matches[2], ad);
			return true;
		}
		return false;
	}

	void DropAction::commitOnTarget(ActionDescriptor* ad, ObjectPointer target) {
		Item* item = target.assertType<Item>("You can drop only an item.")
				.safeCast<Item>();
		LOGS(Debug) << "Dropping " << item->getName() << "." << LOGF;

		if (!item->isDropable()) {
			*ad << "You cannot drop this item.\n" << eos;
			return;
		}

		// Get the backpack
		Inventory* backpack = item->getSingleRelation(R_INVENTORY, Relation::Slave)
				.safeCast<Inventory>();
		if (!backpack) {
			*ad << "You cannot drop this item.\n" << eos;
			return;
		}
		
		if(target->instanceOf(Resource)) {
			Resource* res = target.unsafeCast<Resource>();
			if(amount <= 0 || amount > res->getQuantity()) amount = res->getQuantity();
		}
		else amount = 1;
		ObjectPointer drop = backpack->removeItem(target, amount);
		*ad << "You've dropped " + drop.unsafeCast<IDescriptable>()->getName() + "." << eos;
		ad->getCaller()->getLocation().unsafeCast<Location>()->addItem(drop).unsafeCast<Item>()->onDrop(ad);
	}
}