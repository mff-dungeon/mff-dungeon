#include "Inventory.hpp"
#include "Alive.hpp"
#include "../RandomString.hpp"

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

	Inventory::~Inventory() {

	}
	
	Inventory* Inventory::setMaxSpace(int maxSpace) {
		this->maxSpace = maxSpace;
		return this;
	}
	
	int Inventory::getMaxSpace() {
		return this->maxSpace;
	}
	
	int Inventory::getFreeSpace() {
		return (this->maxSpace - this->usedSpace);
	}
	
	Inventory* Inventory::setMaxWeight(int maxWeight) {
		this->maxWeight = maxWeight;
		return this;
	}
	
	int Inventory::getMaxWeight() {
		return this->maxWeight;
	}
	
	int Inventory::getFreeWeight() {
		return (this->maxWeight - this->usedWeight);
	}

	int Inventory::getBaseSize() {
		return Item::getSize();
	}

	Item* Inventory::setBaseSize(int size) {
		Item::setSize(size);
		return this;
	}

	int Inventory::getSize() {
		return getBaseSize() + usedSpace;
	}

	Item* Inventory::setSize(int size) {
		Item::setSize(size);
		return this;
	}

	int Inventory::getBaseWeight() {
		return Item::getWeight();
	}

	Item* Inventory::setBaseWeight(int weight) {
		Item::setWeight(weight);
		return this;
	}

	int Inventory::getWeight() {
		return getBaseWeight() + usedWeight;
	}

	Item* Inventory::setWeight(int weight) {
		Item::setWeight(weight);
		return this;
	}

	void Inventory::addItem(ObjectPointer itemPtr) {
		itemPtr.assertType<Item>("You can add only items into your backpack.");
		Item* item = itemPtr.safeCast<Item>();
		this->usedWeight += item->getWeight();
		this->usedSpace += item->getSize();
		this->getGameManager()->createRelation(this, item, R_INVENTORY);
		save();
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
		string sentence = "";
		
		try {
			ObjectMap items = this->getRelations(Relation::Master, R_INVENTORY);
			if(items.size() >= 2) {
				int count = 0;
				sentence += "There are ";
				for(auto& item : items) {
					if(count > 0 && count < items.size() - 1) {
						sentence += ", ";
					}
					else if(count == items.size() - 1) {
						sentence += " and ";
					}
					count++;
					if(item.second->instanceOf(IDescriptable)) {
						sentence += item.second.unsafeCast<IDescriptable>()->getName();
					}
					else {
						sentence += item.second.getId();
					}
				}
				sentence += ". ";
			}
			else if(items.size() == 1) {
				for(auto& item : items) {
					sentence += "There is ";
					if(item.second->instanceOf(IDescriptable)) {
						sentence += item.second.unsafeCast<IDescriptable>()->getName();
					}
					else {
						sentence += item.second.getId();
					}
					sentence += ". ";
				}
			}
			else {
				sentence += "It is empty. ";
			}
		}
		catch(const std::out_of_range& e) {
			
		}
		return sentence;
	}
	
	void Inventory::getActions(ActionList* list, ObjectPointer callee) {
		Wearable::getActions(list, callee);
		
		// Drop action, adding only if there is anything to drop
		DropAction* action = new DropAction;
		try {
			ObjectMap itemsIn = this->getRelations(Relation::Master, R_INVENTORY);
			for(auto& item : itemsIn) {
				item.second->getActions(list, callee);
				action->addTarget(item.second);
			}
		}
		catch (const std::out_of_range& e) {
			
		}
		if(action->getTargets().size() > 0) {
			list->addAction(action);
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
		*ad << "Use 'drop ...' to drop items from your backpack. \n";
	}
	
	bool DropAction::matchCommand(string command) {
		return RegexMatcher::match("drop .+", command);
	}
        
	void DropAction::commit(ActionDescriptor* ad) {	
		commitOnBestTarget(ad, ad->in_msg.substr(5));
	}
        
	void DropAction::commitOnTarget(ActionDescriptor* ad, ObjectPointer target) {	
		Item* item = target.assertType<Item>("You can drop only an item.")
				.safeCast<Item>();
		
		if(!item->isDropable()) {
			*ad << "You cannot drop this item.\n";
			return;
		}
		
		// Get the backpack
		Inventory* backpack = item->getSingleRelation(R_INVENTORY, Relation::Slave)
				.safeCast<Inventory>();
		if (!backpack) {
			*ad << "You cannot drop this item.\n";
			return;
		}
		
		backpack->removeItem(target);
		ad->getGM()->createRelation(ad->getAlive()->getLocation(), item, R_INSIDE);
		
		*ad << "You've dropped " + item->getName() + ".";
	}
}