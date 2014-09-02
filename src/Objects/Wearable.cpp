#include "Wearable.hpp"
#include "Inventory.hpp"

namespace Dungeon {

	const char* Wearable::SlotRelations[] = { "equip-backpack", "equip-weapon", "equip-bodyarmor" };
	
	Wearable::Wearable() {
	
	}
	
	Wearable::Wearable(objId id) {
		this->setId(id);
	}

	Wearable::~Wearable() {

	}
	
	Wearable::Slot Wearable::getSlot() const {
		return slot;
	}
	
	Wearable* Wearable::setSlot(Wearable::Slot slot) {
		this->slot = slot;
		return this;
	}

	void Wearable::getActions(ActionList* list, IObject* callee) {
		Item::getActions(list, callee);
		// Do I see this equipped or not?
		try {
			ObjectMap equipped = this->getRelations(Relation::Slave, Wearable::SlotRelations[this->getSlot()]);
			if(equipped.find(callee->getId()) != equipped.end()) {
				// Add unequip action
			}
			else {
				EquipAction* action = new EquipAction;
				action->addTarget(this->getObjectPointer());
				list->addAction(action);
			}
		}
		catch (const std::out_of_range& e) { // Not equipped, well then
			EquipAction* action = new EquipAction;
			action->addTarget(this->getObjectPointer());
			list->addAction(action);
		}
	}

	void EquipAction::explain(ActionDescriptor* ad) {
		*ad << "Use 'wear ...' or 'equip ...' to equip chosen item.";
	}

	bool EquipAction::matchCommand(string command) {
		return RegexMatcher::match("(wear|equip) .+", command);
	}

	void EquipAction::commit(ActionDescriptor* ad) {
		if(ad->in_msg.find("wear") == 0) {
			commitOnBestTarget(ad, ad->in_msg.substr(5));
		}
		else { // equip
			commitOnBestTarget(ad, ad->in_msg.substr(6));
		}
	}
	
	bool EquipAction::unequip(ActionDescriptor* ad, Wearable* item) {
		// Drop / backpack?
		*ad << "Do you want to drop " + item->getName() + ", or do you want to put it into your backpack?";
		ad->waitForReply([item] (ActionDescriptor *ad, string reply) {
			// TODO: better matching the answer
			if(reply.compare("drop") == 0) {
				ad->getGM()->removeRelation(ad->getAlive(), item, Wearable::SlotRelations[item->getSlot()]);
				ad->getGM()->createRelation(ad->getAlive()->getLocation(), item, R_INSIDE);
				*ad << "You have dropped " + item->getName() + ".";
				return;
			}
			else {
				try {
					ObjectMap inventories = ad->getAlive()->getRelations(Relation::Master, Wearable::SlotRelations[Wearable::Slot::Backpack]);
					if(inventories.size() == 0) {
						*ad << "You have no backpack to put the item to.";
						return;
					}
					Inventory* backpack = (Inventory*) inventories.begin()->second.get();
					if(backpack->getFreeSpace() < item->getSize() 
							|| backpack->getFreeWeight() < item->getWeight()) {
						*ad << "You have no free space in your " + backpack->getName() + ".";
						return;
					}
					ad->getGM()->removeRelation(ad->getAlive(), item, Wearable::SlotRelations[item->getSlot()]);
					backpack->addItem(item);
					*ad << "You have unequiped " + item->getName() + " and put it into your " + backpack->getName() + ".";
					return;
				}
				catch (const std::out_of_range& e) {

				}
			}
		});
		// Check if the slot is empty now, otherwise unequip failed
		try {
			ObjectMap equiped = ad->getAlive()->getRelations(Relation::Master, Wearable::SlotRelations[item->getSlot()]);
			if(equiped.size() != 0) {
				return false;
			}
		}
		catch(const std::out_of_range& e) {
			
		}
		// Correct, no item in slot found
		return true;
	}

	void EquipAction::commitOnTarget(ActionDescriptor* ad, ObjectPointer target) {
		Wearable* item = (Wearable*) target.get();
		Wearable* equipedItem = 0;
		// Get current equiped item
		try {
			ObjectMap equiped = ad->getAlive()->getRelations(Relation::Master, Wearable::SlotRelations[item->getSlot()]);
			if(equiped.size() == 1) {
				equipedItem = (Wearable*) equiped.begin()->second.get();
			}
		}
		catch(const std::out_of_range& e) {
			
		}
		
		if(!unequip(ad, equipedItem)) {
			*ad << equipedItem->getName() + " couldn't be unequiped, so you cannot wear " + item->getName() + " now.";
			return;
		}
		
		// Let's equip the item. Where is it though? Because we can equip the things on the ground
		Room* currentRoom = (Room*) ad->getAlive()->getLocation().get();
		if(currentRoom->contains(item)) {
			ad->getGM()->removeRelation(currentRoom, item, R_INSIDE);
			ad->getGM()->createRelation(ad->getAlive(), item, Wearable::SlotRelations[item->getSlot()]);
			*ad << "You have equipped " + item->getName() + ".";
			return;
		}
		// Let's try backpack
		
		try {
			ObjectMap inventories = ad->getAlive()->getRelations(Relation::Master, Wearable::SlotRelations[Wearable::Slot::Backpack]);
			if(inventories.size() > 0) {
				Inventory* backpack = (Inventory*) inventories.begin()->second.get();
				if(backpack->contains(item)) {
					// It is in the backpack, let's remove it
					backpack->removeItem(item);
					ad->getGM()->createRelation(ad->getAlive(), item, Wearable::SlotRelations[item->getSlot()]);
					*ad << "You have equipped " + item->getName() + ".";
				}
			}
		}
		catch(const std::out_of_range& e) {
			
		}
		*ad << item->getName() + " cannot be found so nothing is equipped.";
		return;
	}
	
	void UnequipAction::explain(ActionDescriptor* ad) {
		*ad << "Use 'take off ...' or 'unequip ...' to equip chosen item.";
	}

	bool UnequipAction::matchCommand(string command) {
		return RegexMatcher::match("(take off|unequip) .+", command);
	}

	void UnequipAction::commit(ActionDescriptor* ad) {
		if(ad->in_msg.find("take off") == 0) {
			commitOnBestTarget(ad, ad->in_msg.substr(9));
		}
		else { // unequip
			commitOnBestTarget(ad, ad->in_msg.substr(8));
		}
	}
	
	bool UnequipAction::unequip(ActionDescriptor* ad, Wearable* item) {
		// Drop / backpack?
		*ad << "Do you want to drop " + item->getName() + ", or do you want to put it into your backpack?";
		ad->waitForReply([item] (ActionDescriptor *ad, string reply) {
			// TODO: better matching the answer
			if(reply.compare("drop") == 0) {
				ad->getGM()->removeRelation(ad->getAlive(), item, Wearable::SlotRelations[item->getSlot()]);
				ad->getGM()->createRelation(ad->getAlive()->getLocation(), item, R_INSIDE);
				*ad << "You have dropped " + item->getName() + ".";
				return;
			}
			else {
				try {
					ObjectMap inventories = ad->getAlive()->getRelations(Relation::Master, Wearable::SlotRelations[Wearable::Slot::Backpack]);
					if(inventories.size() == 0) {
						*ad << "You have no backpack to put the item to.";
						return;
					}
					Inventory* backpack = (Inventory*) inventories.begin()->second.get();
					if(backpack->getFreeSpace() < item->getSize() 
							|| backpack->getFreeWeight() < item->getWeight()) {
						*ad << "You have no free space in your " + backpack->getName() + ".";
						return;
					}
					ad->getGM()->removeRelation(ad->getAlive(), item, Wearable::SlotRelations[item->getSlot()]);
					backpack->addItem(item);
					*ad << "You have unequiped " + item->getName() + " and put it into your " + backpack->getName() + ".";
					return;
				}
				catch (const std::out_of_range& e) {

				}
			}
		});
		// Check if the slot is empty now, otherwise unequip failed
		try {
			ObjectMap equiped = ad->getAlive()->getRelations(Relation::Master, Wearable::SlotRelations[item->getSlot()]);
			if(equiped.size() != 0) {
				return false;
			}
		}
		catch(const std::out_of_range& e) {
			
		}
		// Correct, no item in slot found
		return true;
	}
	
	void UnequipAction::commitOnTarget(ActionDescriptor* ad, ObjectPointer target) {
		Wearable* item = (Wearable*) target.get();
		if(!this->unequip(ad, item)) {
			*ad << item->getName() + " couldn't be unequiped.";
		}
	}
	
	PERSISTENT_IMPLEMENTATION(Wearable)
}
