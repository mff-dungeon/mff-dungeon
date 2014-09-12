#include "Wearable.hpp"
#include "Inventory.hpp"
#include "../FuzzyStringMatcher.hpp"

namespace Dungeon {

	const char* Wearable::SlotRelations[] = { "invalid", "equip-weapon", "equip-backpack", "equip-shield",
		"equip-helmet", "equip-boots", "equip-gloves", "equip-bodyarmor" };

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

	int Wearable::getHpBonus() const {
		return hpBonus;
	}

	Wearable* Wearable::setHpBonus(int bonus) {
		this->hpBonus = bonus;
		return this;
	}
	
	string Wearable::getEquippedSentence() const {
		switch (getSlot()) {
			// FIXME do a nicer stuff
			case Slot::Backpack:
				return "As your backpack, you're using " + getName() + ".";
			case Slot::BodyArmor:
			case Slot::Boots:
			case Slot::Gloves:
			case Slot::Helmet:
				return getName() + " gives you some protection for damage.";
			case Slot::Weapon:
				return getName() + " is your main weapon.";
			case Slot::Shield:
				return "Your defense is boosted by " + getName() + ".";
			case Slot::Invalid:
			default:
				return getName() + " is fucking out of our tables for some reason.";
		}
	}
	
	string Wearable::getDescription() const {
		stringstream ss;
		ss << Item::getDescription();
		if (getAttackBonus())
			ss << "It adds " << getAttackBonus() << " to your attack.";
		if (getDefenseBonus())
			ss << "It adds " << getDefenseBonus() << " to your defense.";
		return ss.str();
	}

	void Wearable::getActions(ActionList* list, ObjectPointer callee) {
		Item::getActions(list, callee);
		// Do I see this equipped or not?
		
		if (hasRelation(Wearable::SlotRelations[this->getSlot()], callee, Relation::Slave)) {
			UnequipAction* action = new UnequipAction;
			action->addTarget(this);
			list->addAction(action);
		}
		else {
			EquipAction* action = new EquipAction;
			action->addTarget(this);
			list->addAction(action);
		}
	}

	void Wearable::registerProperties(IPropertyStorage& storage) {
		storage.have((int&) slot, "wearable-slot", "Slot where to wear this item: 1 - Weapon, 2 - Backpack, 3 - Body armor");
		storage.have(attackBonus, "wearable-attack", "Attack bonus of this item");
		storage.have(defenseBonus, "wearable-defense", "Defense bonus of this item");
		storage.have(hpBonus, "wearable-hp", "Hitpoints bonus of this item");
		Item::registerProperties(storage);
	}
	
	/*******************************************************************
					Actions - stage methods 
	 *******************************************************************/
	bool Wearable::unequip(ActionDescriptor* ad, ObjectPointer itemPtr, DesiredAction action) {
		itemPtr.assertType<Wearable>(GameStateInvalid::EquippedNonWearable);
		Wearable* item = itemPtr.unsafeCast<Wearable>();
		
		if(action == DesiredAction::Drop) {
			ad->getGM()->removeRelation(ad->getAlive(), item, Wearable::SlotRelations[item->getSlot()]);
			itemPtr->setSingleRelation(R_INSIDE, ad->getAlive()->getLocation(), Relation::Slave);
			*ad << "You have dropped " + item->getName() + "." << eos;
			item->onUnequip(ad);
			item->onDrop(ad);
			return true;
		}
		else if (action == DesiredAction::Keep) {
			Inventory* backpack = ad->getAlive()->getBackpack()
					.safeCast<Inventory>();
			if(!backpack) {
				*ad << "You have no backpack to put the item to." << eos;
				return false;
			}
			if(backpack->getFreeSpace() < item->getSize() 
					|| backpack->getFreeWeight() < item->getWeight()) {
				*ad << "You have no free space in your " + backpack->getName() + "." << eos;
				return false;
			}
			if(backpack->getId() == item->getId()) {
				*ad << "You cannot put " + item->getName() + " into itself!" << eos;
				return false;
			}
			ad->getGM()->removeRelation(ad->getAlive(), item, Wearable::SlotRelations[item->getSlot()]);
			backpack->addItem(itemPtr);
			*ad << "You have unequiped " + item->getName() + " and put it into your " + backpack->getName() + "." << eos;
			item->onUnequip(ad);
			return true;
		}
		return false;
	}
	
	void UnequipAction::explain(ActionDescriptor* ad) {
		*ad << "take off ... - unequip armor, weapon or whatever." << eos;
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
				*ad << item->getName() + " couldn't be unequiped." << eos;
			}
			else {
				ad->getAlive()->calculateBonuses();
			}
			return;
		}
		
		*ad << "Do you want to drop " + item->getName() + ", or do you want to put it into your backpack?" << eos;
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
					*ad << item->getName() + " couldn't be unequiped." << eos;
				}
				else {
					ad->getAlive()->calculateBonuses()->save();
				}
			});	
	}
	
	void EquipAction::explain(ActionDescriptor* ad) {
		*ad << "Use 'wear ...' or 'equip ...' to equip chosen item." << eos;
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
	
	void EquipAction::commitOnTarget(ActionDescriptor* ad, ObjectPointer target) {
		target.assertType<Wearable>(GameStateInvalid::EquippedNonWearable);
		itemPtr = target;
		Wearable* item = target.unsafeCast<Wearable>();
		
		slot = item->getSlot();
		slotRelation = Wearable::SlotRelations[slot];
		
		equipedItemPtr = ad->getAlive()->getSingleRelation(slotRelation, Relation::Master, GameStateInvalid::EquippedMoreThanOne);
		
		if (!equipedItemPtr) {
			// There's no difficult changing, just equip it
			this->itemPhaseThree(ad);
			return;
		}
		
		equipedItemPtr.assertType<Wearable>(GameStateInvalid::EquippedNonWearable);
		
		// What we need to do?
		if(slot == Wearable::Slot::Backpack) {
			this->backpackPhaseOne(ad);
		}
		else {
			this->itemPhaseOne(ad);
		}
	}

	void EquipAction::itemPhaseOne(ActionDescriptor* ad) {
		*ad << "Do you want to drop " 
			<< equipedItemPtr.safeCast<IDescriptable>()->getName()
			<< ", or do you want to put it into your backpack?" << eos;
		ad->waitForReply([this] (ActionDescriptor *ad, string reply) {
			FuzzyStringMatcher<Wearable::DesiredAction> matcher;
			matcher.add("drop", Wearable::DesiredAction::Drop);
			matcher.add("ground", Wearable::DesiredAction::Drop);
			matcher.add("drop on the ground", Wearable::DesiredAction::Drop);
			matcher.add("keep", Wearable::DesiredAction::Keep);
			matcher.add("put", Wearable::DesiredAction::Keep);
			matcher.add("put in backpack", Wearable::DesiredAction::Keep);
			matcher.add("backpack", Wearable::DesiredAction::Keep);
			this->dAction = matcher.find(reply);
			itemPhaseTwo(ad);
		});
	}	
	
	void EquipAction::itemPhaseTwo(ActionDescriptor* ad) {
		//TODO: Add doc, this one removes current item
		// Get it all again in case it doesn't exist
		// FIXME: Should check, if the action is still valid (someone else did something with the item)
		itemPtr.assertExists("Item which was going to be equiped somehow disappeared");
		equipedItemPtr.assertExists("Item, which was equiped, somehow diasppeared");
		if(!Wearable::unequip(ad, equipedItemPtr, dAction)) {
			*ad << equipedItemPtr.safeCast<IDescriptable>()->getName() 
				<< " couldn't be unequiped, so you cannot wear " 
				<< itemPtr.safeCast<IDescriptable>()->getName() 
				<< " now." << eos;
			return;
		}
		this->itemPhaseThree(ad);
	}
	
	void EquipAction::itemPhaseThree(ActionDescriptor* ad) {
		Location* currentRoom = ad->getAlive()->getLocation().safeCast<Location>();
		if(currentRoom->contains(itemPtr)) {
			ad->getGM()->removeRelation(currentRoom, itemPtr, R_INSIDE);
			ad->getAlive()->setSingleRelation(Wearable::SlotRelations[itemPtr.unsafeCast<Wearable>()->getSlot()], itemPtr, Relation::Master, GameStateInvalid::EquippedMoreThanOne);
			*ad << "You have successfully equipped " << itemPtr.unsafeCast<IDescriptable>()->getName() << "." << eos;
			ad->getAlive()->calculateBonuses()->save();
			itemPtr.unsafeCast<Wearable>()->onPick(ad);
			itemPtr.unsafeCast<Wearable>()->onEquip(ad);
			return;
		}
		
		try {
			Inventory* backpack = ad->getAlive()->getBackpack().safeCast<Inventory>();
			if (backpack) {
				if(backpack->contains(itemPtr)) {
					backpack->removeItem(itemPtr);
					ad->getAlive()->setSingleRelation(slotRelation, itemPtr, Relation::Master, GameStateInvalid::EquippedMoreThanOne);
					*ad << "You have successfully equipped " << itemPtr.unsafeCast<IDescriptable>()->getName() << "." << eos;
					ad->getAlive()->calculateBonuses()->save();
					itemPtr.unsafeCast<Wearable>()->onEquip(ad);
					return;
				}
			}
		}
		catch (const std::out_of_range& e) {
			
		}
		throw GameStateInvalid("The item is somehow unreachable.");
	}

	void EquipAction::backpackPhaseOne(ActionDescriptor* ad) {
		itemPtr.assertType<Inventory>("Item being equipped is not an inventory type.");
		equipedItemPtr.assertType<Inventory>("Item currently equipped is not an inventory type.");
		Inventory* newPack = itemPtr.safeCast<Inventory>();
		Inventory* currentPack = equipedItemPtr.safeCast<Inventory>();
		
		*ad << "Would you like to move your items to a " << newPack->getName() << " and keep " 
				<< currentPack->getName() + ", move items and drop " << currentPack->getName() 
				<< " or leave all items in " << currentPack->getName() << " and drop it?" << eos;
		ad->waitForReply([this] (ActionDescriptor *ad, string reply) {
			FuzzyStringMatcher<Wearable::DesiredAction> matcher;
			matcher.add("move keep", Wearable::DesiredAction::MoveAndKeep);
			matcher.add("move and keep", Wearable::DesiredAction::MoveAndKeep);
			matcher.add("keep", Wearable::DesiredAction::MoveAndKeep);
			matcher.add("move drop", Wearable::DesiredAction::MoveAndDrop);
			matcher.add("move and drop", Wearable::DesiredAction::MoveAndDrop);
			matcher.add("leave", Wearable::DesiredAction::Drop);
			matcher.add("drop", Wearable::DesiredAction::Drop);
			matcher.add("leave and drop", Wearable::DesiredAction::Drop);
			
			dAction = matcher.find(reply);
			this->backpackPhaseTwo(ad);
		});
	}

	void EquipAction::backpackPhaseTwo(ActionDescriptor* ad) {
		// FIXME: check if nothing changed
		itemPtr.assertExists("Item which was going to be equiped somehow disappeared.");
		equipedItemPtr.assertExists("Item, which was equiped, somehow diasppeared.");
		Inventory* newPack = itemPtr.safeCast<Inventory>();
		Inventory* currentPack = equipedItemPtr.safeCast<Inventory>();
		
		if(dAction == Wearable::DesiredAction::NotKnown) {
			*ad << "I don't know what you mean." << eos;
			return;
		}
		else if(dAction == Wearable::DesiredAction::Drop) {
			Wearable::unequip(ad, currentPack, Wearable::DesiredAction::Drop);
			if(currentPack->contains(itemPtr)) {
				currentPack->removeItem(itemPtr);
			}
			backpackPhaseThree(ad);
		}
		else {
			int requiredSize = currentPack->getMaxSpace() - currentPack->getFreeSpace();
			int requiredWeight = currentPack->getMaxWeight() - currentPack->getFreeWeight();
			if(dAction == Wearable::DesiredAction::MoveAndKeep) {
				requiredSize += currentPack->getBaseSize();
				requiredWeight += currentPack->getBaseWeight();
			}
			if(currentPack->contains(itemPtr)) {
				requiredSize -= newPack->getBaseSize();
				requiredWeight -= newPack->getBaseWeight();
			}
			// Do the checks
			if(newPack->getFreeSpace() < requiredSize) {
				*ad << "You cannot switch backpacks because " + newPack->getName() + " isn't big enough." << eos;
				return;
			}
			if(newPack->getFreeWeight() < requiredWeight) {
				*ad << "You cannot switch backpacks because " + newPack->getName() + " cannot hold that much weight." << eos;
				return;
			}
			// Let's move the items
			try {
				ObjectMap inventory = currentPack->getRelations(Relation::Master, R_INVENTORY);
				for(auto& i : inventory) {
					if(!i.second->isInstanceOf(Item::ItemClassName)) continue;
					Item* it = i.second.unsafeCast<Item>();
					if(it != newPack) {
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
			if(dAction == Wearable::DesiredAction::MoveAndDrop) {
				Wearable::unequip(ad, currentPack, Wearable::DesiredAction::Drop);
			} 
			else {
				ad->getGM()->removeRelation(ad->getAlive(), currentPack, Wearable::SlotRelations[Wearable::Slot::Backpack]);
				newPack->addItem(equipedItemPtr);
				*ad << "You have unequiped " << currentPack->getName() 
					<< " and put it into your " << newPack->getName() << "." << eos;
				equipedItemPtr.unsafeCast<Wearable>()->onUnequip(ad);
			}
			backpackPhaseThree(ad);
		}
	}
	void EquipAction::backpackPhaseThree(ActionDescriptor* ad) {
		Inventory* newPack = itemPtr.safeCast<Inventory>();
		Location* currentLoc = ad->getAlive()->getLocation().safeCast<Location>();
		if(currentLoc->contains(itemPtr)) {
			ad->getGM()->removeRelation(currentLoc, newPack, R_INSIDE);
		}
		ad->getGM()->createRelation(ad->getAlive(), newPack, slotRelation);
		*ad << "You have equipped " << newPack->getName() << "." << eos;
		ad->getAlive()->calculateBonuses()->save();	
		newPack->onEquip(ad);
	}
	
	PERSISTENT_IMPLEMENTATION(Wearable)
}
