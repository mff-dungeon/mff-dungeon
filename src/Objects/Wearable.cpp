#include "Wearable.hpp"
#include "Inventory.hpp"
#include "../FuzzyStringMatcher.hpp"

namespace Dungeon {

	const char* Wearable::SlotRelations[] = { "invalid", "equip-weapon", "equip-backpack", "equip-bodyarmor" };

	Wearable::Slot Wearable::getSlot() const {
		return slot;
	}
	
	Wearable* Wearable::setSlot(Wearable::Slot slot) {
		this->slot = slot;
		return this;
	}

	int Wearable::getAttackBonus() const {
		return attackBonus;
	}

	Wearable* Wearable::setAttackBonus(int bonus) {
		this->attackBonus = bonus;
		return this;
	}

	int Wearable::getDefenseBonus() const {
		return defenseBonus;
	}

	Wearable* Wearable::setDefenseBonus(int bonus) {
		this->defenseBonus = bonus;
		return this;
	}

	void Wearable::getActions(ActionList* list, ObjectPointer callee) {
		Item::getActions(list, callee);
		// Do I see this equipped or not?
		try {
			ObjectMap equipped = this->getRelations(Relation::Slave, Wearable::SlotRelations[this->getSlot()]);
			if(equipped.find(callee->getId()) != equipped.end()) {
				UnequipAction* action = new UnequipAction;
				action->addTarget(this->getObjectPointer());
				list->addAction(action);
			}
			else {
				EquipAction* action = new EquipAction;
				action->addTarget(this->getObjectPointer());
				list->addAction(action);
			}
		}
		catch (const std::out_of_range& e) { // Nothing equipped, well then
			EquipAction* action = new EquipAction;
			action->addTarget(this->getObjectPointer());
			list->addAction(action);
		}
	}
	
	/*******************************************************************
					Actions - stage methods 
	 *******************************************************************/
	bool Wearable::unequip(ActionDescriptor* ad, ObjectPointer itemPtr, DesiredAction action) {
		itemPtr.assertType<Wearable>("Unequiped thing must be an item.");
		Wearable* item = itemPtr.unsafeCast<Wearable>();
		
		if(action == DesiredAction::Drop) {
			ad->getGM()->removeRelation(ad->getAlive(), item, Wearable::SlotRelations[item->getSlot()]);
			ad->getGM()->createRelation(ad->getAlive()->getLocation(), item, R_INSIDE);
			*ad << "You have dropped " + item->getName() + ". ";
			return true;
		}
		else if (action == DesiredAction::Keep) {
			try {
				ObjectMap inventories = ad->getAlive()->getRelations(Relation::Master, Wearable::SlotRelations[Wearable::Slot::Backpack]);
				if(inventories.size() == 0) {
					*ad << "You have no backpack to put the item to. ";
					return false;
				}
				Inventory* backpack = inventories.begin()->second.safeCast<Inventory>();
				if(backpack->getFreeSpace() < item->getSize() 
						|| backpack->getFreeWeight() < item->getWeight()) {
					*ad << "You have no free space in your " + backpack->getName() + ". ";
					return false;
				}
				if(backpack->getId() == item->getId()) {
					*ad << "You cannot put " + item->getName() + " into itself! ";
					return false;
				}
				ad->getGM()->removeRelation(ad->getAlive(), item, Wearable::SlotRelations[item->getSlot()]);
				backpack->addItem(item);
				*ad << "You have unequiped " + item->getName() + " and put it into your " + backpack->getName() + ". ";
				return true;
			}
			catch (const std::out_of_range& e) {
			
			}
			return false;
		}
		return false;
	}

	void Wearable::registerProperties(IPropertyStorage& storage) {
		storage.have((int&) slot, "wearable-slot", "Slot where to wear this item: 1 - Weapon, 2 - Backpack, 3 - Body armor");
		storage.have(attackBonus, "wearable-attack", "Attack bonus of this item");
		storage.have(defenseBonus, "wearable-defense", "Defense bonus of this item");
		Item::registerProperties(storage);
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
	
	void EquipAction::equipItem(ActionDescriptor* ad, ObjectPointer itemPtr, ObjectPointer equipedItemPtr, int desiredAction) {
		itemPtr.assertType<Wearable>("Equiped thing must be an item.");
		Wearable* item = itemPtr.unsafeCast<Wearable>();
		
		Wearable* equipedItem = equipedItemPtr.safeCast<Wearable>();
		if(equipedItem) {
			if(!Wearable::unequip(ad, equipedItem, Wearable::DesiredAction::NotKnown)) {
				*ad << equipedItem->getName() + " couldn't be unequiped, so you cannot wear " + item->getName() + " now.";
				return;
			}
		}
		// Let's equip the item.
		Room* currentRoom = ad->getAlive()->getLocation().safeCast<Room>();
		if(currentRoom->contains(item)) {
			ad->getGM()->removeRelation(currentRoom, item, R_INSIDE);
			ad->getGM()->createRelation(ad->getAlive(), item, Wearable::SlotRelations[item->getSlot()]);
			*ad << "You have equipped " + item->getName() + ".";
			ad->getAlive()->calculateBonuses();
			return;
		}
		// Let's try backpack
		
		try {
			ObjectMap inventories = ad->getAlive()->getRelations(Relation::Master, Wearable::SlotRelations[Wearable::Slot::Backpack]);
			if(inventories.size() > 0) {
				Inventory* backpack = inventories.begin()->second.safeCast<Inventory>();
				if(backpack->contains(item)) {
					// It is in the backpack, let's remove it
					backpack->removeItem(item);
					ad->getGM()->createRelation(ad->getAlive(), item, Wearable::SlotRelations[item->getSlot()]);
					*ad << "You have equipped " + item->getName() + ".";
					ad->getAlive()->calculateBonuses();
					return;
				}
			}
		}
		catch(const std::out_of_range& e) {
			
		}
		*ad << item->getName() + " cannot be found so nothing is equipped.";
		return;
	}
	
	void EquipAction::equipBackpack(ActionDescriptor* ad, ObjectPointer newPackPtr, ObjectPointer currentPackPtr) {
		Inventory* newPack = newPackPtr.safeCast<Inventory>();
		Inventory* currentPack = currentPackPtr.safeCast<Inventory>();
		if (!newPack || !currentPack) {
			*ad << newPackPtr.safeCast<IDescriptable>()->getName() << " could not be equipped because of internal error.";
			return;
		}
		
		*ad << "Would you like to move your items to a " + newPack->getName() + " and keep " 
				+ currentPack->getName() + ", move items and drop " + currentPack->getName() 
				+ " or leave all items in " + currentPack->getName()+ " and drop it?";
		ad->waitForReply([currentPack, newPack] (ActionDescriptor *ad, string reply) {
			int drop;
			// FIXME: Use String Matcher
			if(reply.compare("drop") == 0) {
				drop = 3;
			}
			else if(reply.compare("drop and keep") == 0) {
				drop = 2;
			}
			else {
				drop = 1;
			}
			LOG("Wear") << to_string(drop) << LOGF;
			// Move items to new backpack
			if(drop == 3) {
				Wearable::unequip(ad, currentPack, Wearable::DesiredAction::Drop); // drop it
				if(currentPack->contains(newPack)) {
					currentPack->removeItem(newPack);
				}
			}
			else 
			{
				int requiredSize = currentPack->getMaxSpace() - currentPack->getFreeSpace();
				int requiredWeight = currentPack->getMaxWeight() - currentPack->getFreeWeight();
				// Check, if drop the old one
				if(drop == 1) {
					requiredSize += currentPack->getSize();
					requiredWeight += currentPack->getWeight();
				}
				if(currentPack->contains(newPack)) {
					requiredSize -= newPack->getSize();
					requiredSize -= newPack->getWeight();
				}

				if(newPack->getFreeSpace() < requiredSize) {
					*ad << "You cannot switch backpacks because " + newPack->getName() + " isn't big enough.";
					return;
				}
				if(newPack->getFreeWeight() < requiredWeight) {
					*ad << "You cannot switch backpacks because " + newPack->getName() + " cannot hold that much weight.";
					return;
				}
				// Replace the items
				try {
					ObjectMap inventory = currentPack->getRelations(Relation::Master, R_INVENTORY);
					for(auto& i : inventory) {
						if(!i.second->isInstanceOf("Item")) continue;
						Item* it = i.second.unsafeCast<Item>();
						if(it->getId() != newPack->getId()) {
							currentPack->removeItem(it);
							newPack->addItem(it);
						}
						else {
							currentPack->removeItem(it);
						}
					}
				}
				catch (const std::out_of_range& e) {

				}
				// Remove the old backpack
				if(drop == 2) {
					Wearable::unequip(ad, currentPack, Wearable::DesiredAction::Drop);
				} 
				else {
					ad->getGM()->removeRelation(ad->getAlive(), currentPack, Wearable::SlotRelations[Wearable::Slot::Backpack]);
					newPack->addItem(currentPack);
					*ad << "You have unequiped " + currentPack->getName() + " and put it into your " + newPack->getName() + ". ";
				}
			}
					
			//add the new one
			Room* currentLoc = ad->getAlive()->getLocation().safeCast<Room>();
			if(currentLoc->contains(newPack)) {
				ad->getGM()->removeRelation(currentLoc, newPack, R_INSIDE);
			}
			ad->getGM()->createRelation(ad->getAlive(), newPack, Wearable::SlotRelations[newPack->getSlot()]);
			*ad << "You have equipped " + newPack->getName() + ".";
			ad->getAlive()->calculateBonuses();	
		});
	}

	void EquipAction::commitOnTarget(ActionDescriptor* ad, ObjectPointer target) {
		Wearable* item = target.safeCast<Wearable>();
		Wearable* equipedItem = 0;
		// Get current equiped item
		try {
			ObjectMap equiped = ad->getAlive()->getRelations(Relation::Master, Wearable::SlotRelations[item->getSlot()]);
			if(equiped.size() == 1) {
				equipedItem = equiped.begin()->second.safeCast<Wearable>();
			}
		}
		catch(const std::out_of_range& e) {
			
		}
		
		// Needs special treatment
		if(item->getSlot() == Wearable::Slot::Backpack && equipedItem != 0) {
			Inventory* currentPack = (Inventory*) equipedItem;
			Inventory* newPack = (Inventory*) item;
			this->equipBackpack(ad, newPack, currentPack);
		}
		else if (equipedItem != 0) {
			*ad << "Do you want to drop " + equipedItem->getName() + ", or do you want to put it into your backpack?";
			ad->waitForReply([this, item, equipedItem] (ActionDescriptor *ad, string reply) {
				// FIXME: Use StringMatcher to match drop/put...
				if(reply.compare("drop") == 0) {
					this->equipItem(ad, item, equipedItem, 1);
				}
				else {
					this->equipItem(ad, item, equipedItem, 2);
				}
			});	
		}
		else {
			this->equipItem(ad, item);
		}
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
	
	void UnequipAction::commitOnTarget(ActionDescriptor* ad, ObjectPointer target) {
		Wearable* item = target.safeCast<Wearable>();
		// How can we put something in backpack if we won't have it anymore?
		if(item->isInstanceOf(Inventory::InventoryClassName)) {
			if(!Wearable::unequip(ad, item, Wearable::DesiredAction::Drop)) {
				*ad << item->getName() + " couldn't be unequiped.";
			}
			else {
				ad->getAlive()->calculateBonuses();
			}
			return;
		}
		
		*ad << "Do you want to drop " + item->getName() + ", or do you want to put it into your backpack?";
			ad->waitForReply([this, item] (ActionDescriptor *ad, string reply) {
				FuzzyStringMatcher<Wearable::DesiredAction> matcher;
				matcher.add("drop", Wearable::DesiredAction::Drop);
				matcher.add("ground", Wearable::DesiredAction::Drop);
				matcher.add("drop on the ground", Wearable::DesiredAction::Drop);
				matcher.add("keep", Wearable::DesiredAction::Keep);
				matcher.add("put", Wearable::DesiredAction::Keep);
				matcher.add("put in backpack", Wearable::DesiredAction::Keep);
				Wearable::DesiredAction dAction = matcher.find(reply);
				
				if(!Wearable::unequip(ad, item, dAction)) {
					*ad << item->getName() + " couldn't be unequiped.";
				}
				else {
					ad->getAlive()->calculateBonuses()->save();
				}
			});	
	}

	
	PERSISTENT_IMPLEMENTATION(Wearable)
}
