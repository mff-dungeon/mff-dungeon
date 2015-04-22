#include "Wearable.hpp"
#include "../Utils/FuzzyStringMatcher.hpp"
#include "../Utils/RegexMatcher.hpp"
#include "../Game/GameManager.hpp"
#include "Inventory.hpp"
#include "Location.hpp"

namespace Dungeon {

	const char* Wearable::SlotRelations[] = {"invalid", "equip-weapon", "equip-backpack", "equip-shield",
		"equip-helmet", "equip-boots", "equip-gloves", "equip-bodyarmor"};

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
				LOGS(Error) << "Undefined Wearable slot " << getSlot() << "." << LOGF;
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
		if (getHpBonus())
			ss << "It raises your maximum hitpoints by " << getHpBonus() << ".";
		return ss.str();
	}

	void Wearable::getActions(ActionDescriptor *ad) {
		// Do I see this equipped or not?

		if (hasRelation(Wearable::SlotRelations[this->getSlot()], ad->getCaller(), Relation::Slave)) {
			ad->getActionList().addAction(new UnequipAction)
					->addTarget(this);
		} else {
			ad->getActionList().addAction(new EquipAction)
					->addTarget(this)
					->useActionFor(this, ad);
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

		if (action == DesiredAction::Drop) {
			ad->getGM()->removeRelation(ad->getCaller(), item, Wearable::SlotRelations[item->getSlot()]);
			itemPtr->setSingleRelation(R_INSIDE, ad->getCaller()->getLocation(), Relation::Slave);
			*ad << "You have dropped " + item->getName() + "." << eos;
			item->onUnequip(ad);
			item->onDrop(ad);
			return true;
		} else if (action == DesiredAction::Keep) {
			Inventory* backpack = ad->getCaller()->getBackpack()
					.safeCast<Inventory>();
			if (!backpack) {
				*ad << "You have no backpack to put the item to." << eos;
				return false;
			}
			if (backpack->getFreeSpace() < item->getSize()
					|| backpack->getFreeWeight() < item->getWeight()) {
				*ad << "You have no free space in your " + backpack->getName() + "." << eos;
				return false;
			}
			if (backpack->getId() == item->getId()) {
				*ad << "You cannot put " + item->getName() + " into itself!" << eos;
				return false;
			}
			ad->getGM()->removeRelation(ad->getCaller(), item, Wearable::SlotRelations[item->getSlot()]);
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

	bool UnequipAction::match(const string& command, ActionDescriptor* ad) {
		RegexMatcher::matches matches;
		if (RegexMatcher::match("(take off|unequip)( (.+))?", command, matches)) {
			selectBestTarget(matches[3], ad);
			return true;
		}
		return false;
	}

	void UnequipAction::commitOnTarget(ActionDescriptor* ad, ObjectPointer target) {
		Wearable* item = target.safeCast<Wearable>();
		// How can we put something in backpack if we won't have it anymore?
		if (item->instanceOf(Inventory)) {
			if (!Wearable::unequip(ad, item, Wearable::DesiredAction::Drop)) {
				*ad << item->getName() + " couldn't be unequiped." << eos;
			} else {
				ad->getCaller()->calculateBonuses();
			}
			return;
		}

		*ad << "Do you want to drop " + item->getName() + ", or do you want to put it into your backpack?" << eos;
		ad->waitForReply([this, item] (ActionDescriptor *ad, string reply) {
			FuzzyStringMatcher<Wearable::DesiredAction> matcher;
			matcher.add("drop", Wearable::DesiredAction::Drop)
					.add("ground", Wearable::DesiredAction::Drop)
					.add("drop on the ground", Wearable::DesiredAction::Drop)
					.add("keep", Wearable::DesiredAction::Keep)
					.add("put", Wearable::DesiredAction::Keep)
					.add("put in backpack", Wearable::DesiredAction::Keep);
					Wearable::DesiredAction dAction = matcher.find(reply);

			if (!Wearable::unequip(ad, item, dAction)) {
				*ad << item->getName() + " couldn't be unequiped." << eos;
			} else {
				ad->getCaller()->calculateBonuses()->save();
			}
		});
	}

	void EquipAction::explain(ActionDescriptor* ad) {
		*ad << "Use 'wear ...' or 'equip ...' to equip chosen item." << eos;
	}

	bool EquipAction::match(const string& command, ActionDescriptor* ad) {
		RegexMatcher::matches matches;
		if (RegexMatcher::match("(wear|equip)( (.+))?", command, matches)) {
			selectBestTarget(matches[3], ad);
			return true;
		}
		return false;
	}

	void EquipAction::commitOnTarget(ActionDescriptor* ad, ObjectPointer target) {
		target.assertType<Wearable>(GameStateInvalid::EquippedNonWearable);
		itemPtr = target;
		Wearable* item = target.unsafeCast<Wearable>();
		if (!item->checkStatReqs(ad->getCaller(), ad)) {
			*ad << "You cannot equip " << item->getName() << "." << eos;
			return;
		}
		slot = item->getSlot();
		slotRelation = Wearable::SlotRelations[slot];

		equipedItemPtr = ad->getCaller()->getSingleRelation(slotRelation, Relation::Master, GameStateInvalid::EquippedMoreThanOne);

		if (!equipedItemPtr) {
			// There's no difficult changing, just equip it
			this->itemPhaseThree(ad);
			return;
		}

		equipedItemPtr.assertType<Wearable>(GameStateInvalid::EquippedNonWearable);

		// What we need to do?
		if (slot == Wearable::Slot::Backpack) {
			this->backpackPhaseOne(ad);
		} else {
			this->itemPhaseOne(ad);
		}
	}

	void EquipAction::itemPhaseOne(ActionDescriptor* ad) {
		Wearable* curUsed = equipedItemPtr.safeCast<Wearable>();
		*ad << "You are currently using " << curUsed->getLongName() << "." << eos;
		*ad << "Do you want to drop " << curUsed->getName() << ", or do you want to put it into your backpack?" << eos;
		ad->waitForReply([this] (ActionDescriptor *ad, string reply) {
			FuzzyStringMatcher<Wearable::DesiredAction> matcher;
			matcher.add("drop", Wearable::DesiredAction::Drop)
					.add("ground", Wearable::DesiredAction::Drop)
					.add("drop on the ground", Wearable::DesiredAction::Drop)
					.add("keep", Wearable::DesiredAction::Keep)
					.add("put", Wearable::DesiredAction::Keep)
					.add("put in backpack", Wearable::DesiredAction::Keep)
					.add("backpack", Wearable::DesiredAction::Keep);
			this->dAction = matcher.find(reply);
			itemPhaseTwo(ad);
		});
	}

	void EquipAction::itemPhaseTwo(ActionDescriptor* ad) {
		// Get it all again in case it doesn't exist
		// FIXME: Should check, if the action is still valid (someone else did something with the item)
		itemPtr.assertExists("Item which was going to be equiped somehow disappeared");
		equipedItemPtr.assertExists("Item, which was equiped, somehow diasppeared");
		if (!Wearable::unequip(ad, equipedItemPtr, dAction)) {
			*ad << equipedItemPtr.safeCast<IDescriptable>()->getName()
					<< " couldn't be unequiped, so you cannot wear "
					<< itemPtr.safeCast<IDescriptable>()->getName()
					<< " now." << eos;
			return;
		}
		this->itemPhaseThree(ad);
	}

	void EquipAction::itemPhaseThree(ActionDescriptor* ad) {
		// Need a way to find/check the item and remove it from there
		bool found = false;
		ObjectPointer inventory = itemPtr->getSingleRelation(R_INVENTORY, Relation::Slave);
		if(!!inventory) {
			inventory.assertType<Inventory>().unsafeCast<Inventory>()->removeItem(itemPtr);
			found = true;
		}
		else {
			ObjectPointer location = itemPtr->getSingleRelation(R_INSIDE, Relation::Slave);
			if(!!location) {
				location.assertType<Location>();
				ad->getGM()->removeRelation(location, itemPtr, R_INSIDE);
				found = true;
			}
		}
		// FIXME: any way to find it somehow? We need to ensure it's still at the same location as it was when the action was initiated
		if(!found) {
			throw GameStateInvalid("The item is somehow unreachable.");
			return;
		}
		ad->getCaller()->setSingleRelation(Wearable::SlotRelations[itemPtr.unsafeCast<Wearable>()->getSlot()], itemPtr, Relation::Master, GameStateInvalid::EquippedMoreThanOne);
		*ad << "You have successfully equipped " << itemPtr.unsafeCast<IDescriptable>()->getName() << "." << eos;
		ad->getCaller()->calculateBonuses()->save();
		itemPtr.unsafeCast<Wearable>()->onPick(ad);
		itemPtr.unsafeCast<Wearable>()->onEquip(ad);
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
			matcher.add("move keep", Wearable::DesiredAction::MoveAndKeep)
					.add("move and keep", Wearable::DesiredAction::MoveAndKeep)
					.add("keep", Wearable::DesiredAction::MoveAndKeep)
					.add("move drop", Wearable::DesiredAction::MoveAndDrop)
					.add("move and drop", Wearable::DesiredAction::MoveAndDrop)
					.add("leave", Wearable::DesiredAction::Drop)
					.add("drop", Wearable::DesiredAction::Drop)
					.add("leave and drop", Wearable::DesiredAction::Drop);

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

		if (dAction == Wearable::DesiredAction::NotKnown) {
			*ad << "I don't know what you mean." << eos;
			return;
		} else if (dAction == Wearable::DesiredAction::Drop) {
			Wearable::unequip(ad, currentPack, Wearable::DesiredAction::Drop);
			if (currentPack->contains(itemPtr)) {
				currentPack->removeItem(itemPtr);
			}
			backpackPhaseThree(ad);
		} else {
			int requiredSize = currentPack->getMaxSpace() - currentPack->getFreeSpace();
			int requiredWeight = currentPack->getMaxWeight() - currentPack->getFreeWeight();
			if (dAction == Wearable::DesiredAction::MoveAndKeep) {
				requiredSize += currentPack->getBaseSize();
				requiredWeight += currentPack->getBaseWeight();
			}
			if (currentPack->contains(itemPtr)) {
				requiredSize -= newPack->getBaseSize();
				requiredWeight -= newPack->getBaseWeight();
			}
			// Do the checks
			if (newPack->getFreeSpace() < requiredSize) {
				*ad << "You cannot switch backpacks because " + newPack->getName() + " isn't big enough." << eos;
				return;
			}
			if (newPack->getFreeWeight() < requiredWeight) {
				*ad << "You cannot switch backpacks because " + newPack->getName() + " cannot hold that much weight." << eos;
				return;
			}
			// Let's move the items
			try {
				const ObjectMap& inventory = currentPack->getRelations(Relation::Master, R_INVENTORY);
				for (auto& i : inventory) {
					if (!i.second->instanceOf(Item)) continue;
					Item* it = i.second.unsafeCast<Item>();
					if (it != newPack) {
						currentPack->removeItem(it);
						newPack->addItem(it);
					} else {
						currentPack->removeItem(it);
					}
				}
			} catch (const std::out_of_range& e) {

			}

			// Remove the old backpack
			if (dAction == Wearable::DesiredAction::MoveAndDrop) {
				Wearable::unequip(ad, currentPack, Wearable::DesiredAction::Drop);
			}
			else {
				ad->getGM()->removeRelation(ad->getCaller(), currentPack, Wearable::SlotRelations[Wearable::Slot::Backpack]);
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
		ObjectPointer itemLoc = newPack->getSingleRelation(R_INSIDE, Relation::Slave);
		if(!!itemLoc) {
			itemLoc.assertType<Location>();
			ad->getGM()->removeRelation(itemLoc, itemPtr, R_INSIDE);
		}
		ad->getGM()->createRelation(ad->getCaller(), newPack, slotRelation);
		*ad << "You have equipped " << newPack->getName() << "." << eos;
		ad->getCaller()->calculateBonuses()->save();
		newPack->onEquip(ad);
	}

	PERSISTENT_IMPLEMENTATION(Wearable)
}
