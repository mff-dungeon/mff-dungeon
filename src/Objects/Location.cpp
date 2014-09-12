#include <exception>

#include "Location.hpp"
#include "../ObjectPointer.hpp"
#include "../ObjectGroup.hpp"
#include "../ActionDescriptor.hpp"
#include "Item.hpp"
#include "Inventory.hpp"
#include "../RandomString.hpp"
#include "Wearable.hpp"
#include "../SentenceJoiner.hpp"

namespace Dungeon {

	bool Location::contains(ObjectPointer object) {
		return hasRelation(R_INSIDE, object, Relation::Master);
	}

	bool Location::isRespawnable() const {
		return respawnable;
	}

	Location* Location::setRespawnable(bool respawnable) {
		this->respawnable = respawnable;
		return this;
	}
	
	string Location::getEmptyMessage() const {
		return emptyMessage != "" ? emptyMessage
				: RandomString::get()
					<< "It is empty. " << endr
					<< "There is nothing. " << endr;
	}
	
	Location* Location::setEmptyMessage(string emptyMessage) {
		this->emptyMessage = emptyMessage;
		return this;
	}

	void Location::getActions(ActionList* list, ObjectPointer callee) {
		LOGS("Location", Verbose) << "Getting actions on " << this->getName() << "." << LOGF;
		// Recursively search all items in this room
		try {
			ObjectMap objects = getRelations(Relation::Master, R_INSIDE);
			for (auto& item : objects) {
				if (item.second != callee)
					item.second->triggerTraps("get-actions", nullptr)
					->getActions(list, callee);
			}
		}		catch (const std::out_of_range& e) {

		}

		// Add pickup for items
		PickupAction* pickAction = new PickupAction;
		try {
			ObjectMap itemsIn = getRelations(Relation::Master, R_INSIDE);
			for (auto& itemObj : itemsIn) {
				if (itemObj.second->instanceOf(Item)) {
					pickAction->addTarget(itemObj.second);
				}
			}
		}		catch (const std::out_of_range& e) {

		}
		if (pickAction->getTargets().size() > 0) {
			list->addAction(pickAction);
		}
		
		IDescriptable::getActions(list, callee);
	}

	string Location::getDescriptionSentence() {
		string common = this->getLongName() + ". " + this->getDescription();
		return RandomString::get()
				<< "There is also " + common << endr
				<< "You can see " + common << endr
				<< "You've searched " + this->getLongName() + " too. " + this->getDescription() << endr;
	}
	
	string Location::getInsideSentence() {
		string common = this->getLongName() + ". " + this->getDescription();
		return RandomString::get()
				<< "You are in " + common << endr
				<< "You find yourself in " + common << endr
				<< "Seems like you are in " + common << endr;
	}

	void Location::examine(ActionDescriptor* ad) {
		LOGS("Location", Verbose) << "Exploring " << this->getName() << "." << LOGF;
		triggerTraps("examine", ad);
		if (!ad) return;
		ObjectPointer alive = ad->getAlive();
		// Recursively search all items in this room
		ObjectGroup groupedObjects;
		SentenceJoiner nested;

		// remove myself from the exploration group
		for (auto obj :  getRelations(Relation::Master, R_INSIDE)) {
			if (alive == obj.second) {
				*ad << getInsideSentence();
			} else if (obj.second->instanceOf(Location)) {
				nested << obj.second;
			} else {
				groupedObjects.insertObject(obj.second);
			}
		}
		
		*ad << groupedObjects.explore();
		*ad << nested.getSentence("", "You may want too look into %.");
		
		if (nested.size() + groupedObjects.size() == 0)
			*ad << getEmptyMessage();
		
	}

	void Location::registerProperties(IPropertyStorage& storage) {
		storage.have(respawnable, "location-respawnable", "True if user can respawn here")
			.have(emptyMessage, "location-emptymessage", "Message that shows when the location is empty.");
		IDescriptable::registerProperties(storage);
	}

	PERSISTENT_IMPLEMENTATION(Location)


	void PickupAction::explain(ActionDescriptor* ad) {
		*ad << "Use 'pick up ...' to pick the items you see.";
	}

	bool PickupAction::matchCommand(string command) {
		return RegexMatcher::match("(pick( up)?|take) .+", command);
	}

	void PickupAction::commit(ActionDescriptor* ad) {
		if (ad->in_msg.find("pick up ") == 0) {
			commitOnBestTarget(ad, ad->in_msg.substr(8));
		} else { // take | pick 
			commitOnBestTarget(ad, ad->in_msg.substr(5));
		}
	}

	void PickupAction::commitOnTarget(ActionDescriptor* ad, ObjectPointer target) {
		Item* item = target.assertType<Item>("You can pick up only an item.").unsafeCast<Item>();
		if (!item->isPickable()) {
			*ad << "You cannot pick " << item->getName() << ". \n";
			return;
		}
		
		ObjectPointer backpack = ad->getAlive()
				->getBackpack();

		if (!backpack) {
			*ad << "You have no inventory to put " << item->getName() << " in. ";
			return;
		}
		
		Inventory* inventory = backpack
				.assertType<Inventory>(GameStateInvalid::BackpackNotInventory)
				.unsafeCast<Inventory>();


		if (inventory->getFreeWeight() < item->getWeight()) {
			*ad << "Content of " << inventory->getName() << " would be too heavy with " << item->getName() << ". ";
			*ad << item->getName() << " weights " << Utils::weightStr(item->getWeight())
					<< ", but there's only " << Utils::weightStr(inventory->getFreeWeight()) << " available.";
			return;
		}

		if (inventory->getFreeSpace() < item->getSize()) {
			*ad << "There is not enough space left for " << item->getName() << " in " << inventory->getName() << ". ";
			return;
		}
		// Everything is allright, let's add it
		Location* current = item->getSingleRelation(R_INSIDE, Relation::Slave, "The item is located in more than one location.")
				.safeCast<Location>();
		if (!current) {
			LOGS("PickupAction", Error) << "The item is nowhere?!" << LOGF;
			return;
		}
		ad->getGM()->removeRelation(current, item, R_INSIDE);
		inventory->addItem(item);
		*ad << "You've picked up " + item->getName() + ".";
		item->onPick(ad);
	}
}