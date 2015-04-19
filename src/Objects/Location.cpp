#include <exception>
#include "../Utils/RandomString.hpp"
#include "../Utils/SentenceJoiner.hpp"
#include "../Utils/RegexMatcher.hpp"
#include "../Game/ObjectPointer.hpp"
#include "../Game/ObjectGroup.hpp"
#include "../Game/ActionDescriptor.hpp"
#include "Location.hpp"
#include "Item.hpp"
#include "Inventory.hpp"
#include "Wearable.hpp"
#include "../Persistence/Cloner.hpp"

using namespace std;

namespace Dungeon {

	bool Location::contains(ObjectPointer object) {
		return hasRelation(R_INSIDE, object, Relation::Master);
	}

	ObjectPointer Location::addItem(ObjectPointer object) {
		object.assertType<Item>();
		LOGS(Debug) << "Adding item "
				<< (object->instanceOf(IDescriptable) ? object.unsafeCast<IDescriptable>()->getName() : object.getId())
				<< " to " << getName() << "." << LOGF;
		if(object->instanceOf(Resource)) {
			Resource* old = object.unsafeCast<Resource>();
			try {
				for(auto& rel : getRelations(Relation::Master, R_INSIDE)) {
					if(rel.second->instanceOf(Resource)) {
						Resource* r = rel.second.unsafeCast<Resource>();
						if(r->getType() == old->getType()) {
							return r->join(object);
						}
					}
				}
			} catch(out_of_range& e) {}
		}
		getGameManager()->createRelation(this, object, R_INSIDE);
		return object;
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
				<< "It is empty." << endr
				<< "There is nothing." << endr;
	}

	Location* Location::setEmptyMessage(const string& emptyMessage) {
		this->emptyMessage = emptyMessage;
		return this;
	}

	void Location::getActions(ActionDescriptor* ad) {
		delegateGetActions(ad, R_INSIDE);

		// Add pickup for items
		PickupAction* pickAction = new PickupAction;
		try {
			const ObjectMap& itemsIn = getRelations(Relation::Master, R_INSIDE);
			for (auto& itemObj : itemsIn) {
				if (ad->getCaller()->hasRelation(R_SEEN, itemObj.second) && itemObj.second->instanceOf(Item)) {
					pickAction->addTarget(itemObj.second);
				}
			}
		} catch (const std::out_of_range& e) {

		}
		if (pickAction->getTargets().size() > 0) {
			ad->getActionList().addAction(pickAction);
		} else {
			delete pickAction;
		}
	}

	string Location::getDescriptionSentence() {
		string common = this->getLongName() + ". " + this->getDescription();
		return RandomString::get()
				<< "There is also " + common << endr
				<< "You can see " + common << endr
				<< "You've searched " + this->getLongName() + " too." + this->getDescription() << endr;
	}

	string Location::getInsideSentence() {
		string name = this->getLongName();
		return RandomString::get()
				<< "You are in " << name << "." << endr
				<< "You find yourself in " << name << "." << endr
				<< "Seems like you are in " << name << "." << endr;
	}

	void Location::examine(ActionDescriptor* ad) {
		LOGS(Debug) << "Exploring " << this->getName() << "." << LOGF;
		triggerTraps("examine", ad);
		if (!ad) return;
		ObjectPointer alive = ad->getCaller();
		// Recursively search all items in this room
		ObjectGroup groupedObjects;
		SentenceJoiner nested;

		// remove myself from the exploration group
		for (auto& obj : getRelations(Relation::Master, R_INSIDE)) {
			if (alive == obj.second) {
				*ad << getInsideSentence() << eos;
				*ad << getDescription() << eos;
			} else if (obj.second->instanceOf(Location)) {
				nested << obj.second;
			} else {
				groupedObjects.insertObject(obj.second);
			}
		}

		groupedObjects.explore(ad, true, alive.safeCast<Human>());
		*ad << nested.getSentence("", "You may want to look into %.") << eos;

		if (nested.size() + groupedObjects.size() == 0)
			*ad << getEmptyMessage() << eos;

	}

	void Location::registerProperties(IPropertyStorage& storage) {
		storage.have(respawnable, "location-respawnable", "True if user can respawn here")
				.have(emptyMessage, "location-emptymessage", "Message that shows when the location is empty.");
		IDescriptable::registerProperties(storage);
	}

	PERSISTENT_IMPLEMENTATION(Location)


	void PickupAction::explain(ActionDescriptor* ad) {
		*ad << "Use 'pick up ...' to pick the items you see." << eos;
	}

	bool PickupAction::match(const string& command, ActionDescriptor* ad) {
		if (captureMatcher.match("(pick( up)?|take|grab|obtain|catch|seize|get)( ([0-9]+ )?(.*))?", command)) {
			try {
				amount = stoi(captureMatcher.matches[4]);
			} catch (invalid_argument& e) {}
			selectBestTarget(captureMatcher.matches[5], ad);
			return true;
		}
		return false;
	}

	void PickupAction::commitOnTarget(ActionDescriptor* ad, ObjectPointer target) {
		Item* item = target.assertType<Item>("You can pick up only an item.").unsafeCast<Item>();
		if (!item->isPickable()) {
			*ad << "You cannot pick " << item->getName() << ". \n" << eos;
			return;
		}

		ObjectPointer backpack = ad->getCaller()
				->getBackpack();

		if (!backpack) {
			*ad << "You have no inventory to put " << item->getName() << " in." << eos;
			return;
		}

		Inventory* inventory = backpack
				.assertType<Inventory>(GameStateInvalid::BackpackNotInventory)
				.unsafeCast<Inventory>();


		ObjectPointer current = item->getSingleRelation(R_INSIDE, Relation::Slave, "The item is located in more than one location.")
				.assertType<Location>("Where the fuck is the item?");

		ObjectPointer orig = target;
		bool split = false;
		if (item->instanceOf(Resource)) {
			Resource* r = (Resource*) item;
			if(r->getQuantity() > amount && amount > 0) {
				target = r->split(amount);
				target.assertExists("The splitting has failed.");
				item = target.unsafeCast<Item>();
				split = true;
			} else if (amount != 0) {
				*ad << "There's just " << r->getName() << " to pick." << eos;
				return;
			}
		}

		if (inventory->getFreeWeight() < item->getWeight()) {
			*ad << "Content of " << inventory->getName() << " would be too heavy with " << item->getName() << "." << eos;
			*ad << item->getName() << " weights " << Utils::weightStr(item->getWeight())
					<< ", but there's only " << Utils::weightStr(inventory->getFreeWeight()) << " available." << eos;
			if(split) orig.unsafeCast<Resource>()->join(target);
			return;
		}

		if (inventory->getFreeSpace() < item->getSize()) {
			*ad << "There is not enough space left for " << item->getName() << " in " << inventory->getName() << "." << eos;
			if(split) orig.unsafeCast<Resource>()->join(target);
			return;
		}
		// Everything is alright, let's add it
		ad->getGM()->removeRelation(current, item, R_INSIDE);
		*ad << "You've picked " + item->getName() + " up." << eos;
		inventory->addItem(item).unsafeCast<Item>()->onPick(ad);
	}
}
