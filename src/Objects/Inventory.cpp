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
	
	Inventory::Inventory(objId id) {
		this->setId(id);
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
	
	void Inventory::addItem(Item* item) {
		this->usedWeight += item->getWeight();
		this->usedSpace += item->getSize();
		this->getGameManager()->createRelation(this, item, R_INVENTORY);
		save();
	}
	
	void Inventory::removeItem(Item* item) {
		this->usedWeight -= item->getWeight();
		this->usedSpace -= item->getSize();
		this->getGameManager()->removeRelation(this, item, R_INVENTORY);
		save();
	}

	bool Inventory::contains(Item* item) {
		// If it throws an error, it's because there is nothing, so it should be safe to return false
		try {
			ObjectMap inside = this->getRelations(Relation::Master, R_INVENTORY);
			if(inside.find(item->getId()) != inside.end())
				return true;
		}
		catch (const std::out_of_range& e) {
			
		}
		return false;
	}
	
	string Inventory::getContainingSentence() {
		string sentence;
        
		sentence = RandomString::get()
			<< "You own a " + this->getName() + ". " << endr
            << "You have a " + this->getName() + " with you. " << endr;
		
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
					if(item.second.get()->instanceOf(IDescriptable)) {
						IDescriptable* itemDesc = (IDescriptable*) item.second.get();
						sentence += itemDesc->getName();
					}
					else {
						sentence += item.second.getId();
					}
				}
				sentence += ".";
			}
			else if(items.size() == 1) {
				for(auto& item : items) {
					sentence += "There is ";
					if(item.second.get()->instanceOf(IDescriptable)) {
						IDescriptable* itemDesc = (IDescriptable*) item.second.get();
						sentence += itemDesc->getName();
					}
					else {
						sentence += item.second.getId();
					}
					sentence += ".";
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
	
	void Inventory::getActions(ActionList* list, IObject* callee) {
		Wearable::getActions(list, callee);
		
		// Drop action, adding only if there is anything to drop
		DropAction* action = new DropAction;
		try {
			ObjectMap itemsIn = this->getRelations(Relation::Master, R_INVENTORY);
			for(auto& item : itemsIn) {
				item.second.get()->getActions(list, callee);
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
		// Presumes the target is instance of item
		Item* item = (Item*) target.get();
		
		if(!item->isDropable()) {
			*ad << "You cannot drop this item.\n";
			return;
		}
		
		// Get the backpack
		Inventory* backpack;
		try {
			ObjectMap backpacks = item->getRelations(Relation::Slave, R_INVENTORY);
			if(backpacks.size() == 0) {
				*ad << "You cannot drop this item.\n";
				return;
			}
			backpack = (Inventory*) backpacks.begin()->second.get();
		}
		catch (const std::out_of_range& e) {
			
		}
		
		backpack->removeItem(item);
		ad->getGM()->createRelation(ad->getAlive()->getLocation().get(), item, R_INSIDE);
		
		*ad << "You've dropped " + item->getName() + ".";
	}
}