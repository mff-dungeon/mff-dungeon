#include "Inventory.hpp"
#include "Alive.hpp"
#include "../Utils/RandomString.hpp"
#include "../Utils/SentenceJoiner.hpp"
#include "../Utils/RegexMatcher.hpp"
#include "../Game/GameManager.hpp"
#include <unistd.h>

namespace Dungeon {

	Inventory::Inventory() {
		this->maxSpace = DEFAULT_SPACE;
		this->maxWeight = DEFAULT_WEIGHT;
		this->usedSpace = 0;
		this->usedWeight = 0;
	}

	Inventory::Inventory(objId id) : Wearable(id) {
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

	void Inventory::addItem(ObjectPointer itemPtr) {
		itemPtr.assertType<Item>("You can add only items into your backpack.");
		Item* item = itemPtr.safeCast<Item>();
		this->usedWeight += item->getWeight();
		this->usedSpace += item->getSize();
		this->getGameManager()->createRelation(this, item, R_INVENTORY);
		save();
	}

	bool Inventory::canAdd(ObjectPointer itemPtr) {
		itemPtr.assertType<Item>("You can add only items into your backpack.");
		Item* item = itemPtr.safeCast<Item>();
		return (getFreeWeight() >= item->getWeight() && getFreeSpace() >= item->getSize());
	}

	void Inventory::removeItem(ObjectPointer itemPtr) {
		itemPtr.assertType<Item>("You can add only items into your backpack.");
		Item* item = itemPtr.safeCast<Item>();
		this->usedWeight -= item->getWeight();
		this->usedSpace -= item->getSize();
		this->getGameManager()->removeRelation(this, item, R_INVENTORY);
		save();
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

	void Inventory::getActions(ActionList* list, ObjectPointer callee) {
		Wearable::getActions(list, callee);

		// Drop action, adding only if there is anything to drop
		DropAction* action = new DropAction;
		try {
			const ObjectMap& itemsIn = this->getRelations(Relation::Master, R_INVENTORY);
			for (auto& item : itemsIn) {
				item.second->getActions(list, callee);
				action->addTarget(item.second);
			}
		} catch (const std::out_of_range& e) {

		}
		if (action->getTargets().size() > 0) {
			list->addAction(action);
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

	bool DropAction::match(string command, ActionDescriptor* ad) {
		if (RegexMatcher::match("drop .+", command)) {
			selectBestTarget(command.substr(5), ad);
			return true;
		}
		return false;
	}

	void DropAction::commitOnTarget(ActionDescriptor* ad, ObjectPointer target) {
		Item* item = target.assertType<Item>("You can drop only an item.")
				.safeCast<Item>();

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

		backpack->removeItem(target);
		ad->getGM()->createRelation(ad->getCaller()->getLocation(), item, R_INSIDE);
		*ad << "You've dropped " + item->getName() + "." << eos;
		item->onDrop(ad);
	}
}