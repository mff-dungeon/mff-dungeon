#include <exception>

#include "Room.hpp"
#include "../ObjectPointer.hpp"
#include "../ObjectGroup.hpp"
#include "../ActionDescriptor.hpp"
#include "Item.hpp"
#include "Inventory.hpp"
#include "../RandomString.hpp"
#include "Wearable.hpp"

namespace Dungeon {

	bool Room::contains(ObjectPointer object) {
		ObjectMap& inside = getRelations(Relation::Master, R_INSIDE);
		return inside.find(object->getId()) != inside.end();
	}

	bool Room::isRespawnable() const {
		return respawnable;
	}

	Room* Room::setRespawnable(bool respawnable) {
		this->respawnable = respawnable;
		return this;
	}

	void Room::getActions(ActionList* list, ObjectPointer callee) {
		LOGS("Room", Verbose) << "Getting actions on " << this->getName() << "." << LOGF;
		// Recursively search all items in this room
		try {
			ObjectMap objects = getRelations(Relation::Master, R_INSIDE);
			for(auto& item: objects) {
				if (item.second.getId() != callee->getId())
					item.second->getActions(list, callee);
			}
		}
		catch (const std::out_of_range& e) {
			
		}
		
		// Add pickup for items
		PickupAction* pickAction = new PickupAction;
		try {
			ObjectMap itemsIn = getRelations(Relation::Master, R_INSIDE);
			for(auto& itemObj: itemsIn) {
				if(itemObj.second->instanceOf(Item)) {
					pickAction->addTarget(itemObj.second);
				}
			}
		}
		catch (const std::out_of_range& e) {
			
		}
		if(pickAction->getTargets().size() > 0) {
			list->addAction(pickAction);
		}
    }
    
    string Room::getDescriptionSentence() {
		string common = this->getLongName() + ". " + this->getDescription();
        return RandomString::get()
                << "You are in " + common << endr
				<< "You find yourself in " + common << endr
				<< "Seems like you are in " + common << endr;
    }
	
	void Room::explore(ActionDescriptor* ad) {
		LOGS("Room", Verbose) << "Exploring " << this->getName() << "." << LOGF;
		*ad << this->getDescriptionSentence() << " ";
		// Recursively search all items in this room
		try{
			ObjectMap objects = getRelations(Relation::Master, R_INSIDE);
            ObjectGroup groupedObjects(objects);
            
            // remove myself from the exploration group
            for (ObjectGroup::iterator it = groupedObjects.begin(); it != groupedObjects.end(); it++) {
                if (it->second.operator==(ad->getAlive())) {
                    groupedObjects.erase(it);
                    break;
                }
            }
            
            *ad << groupedObjects.explore();
		}
		catch (const std::out_of_range& e) {
			
		}
    }
	
	void Room::registerProperties(IPropertyStorage& storage) {
		storage.have(respawnable, "room-respawnable", "True if user can respawn here");
		IDescriptable::registerProperties(storage);
	}

	PERSISTENT_IMPLEMENTATION(Room)


	void PickupAction::explain(ActionDescriptor* ad) {
		*ad << "Use 'pick up ...' to pick the items you see.";
	}
	
	bool PickupAction::matchCommand(string command) {
		return RegexMatcher::match("(pick( up)?|take) .+", command);
	}

	void PickupAction::commit(ActionDescriptor* ad) {
		if(ad->in_msg.find("pick up ") == 0) {
			commitOnBestTarget(ad, ad->in_msg.substr(8));
		}
		else { // take | pick 
			commitOnBestTarget(ad, ad->in_msg.substr(5));
		}
	}

	void PickupAction::commitOnTarget(ActionDescriptor* ad, ObjectPointer target) {
		Item* item = target.assertType<Item>("You can pick up only an item.").unsafeCast<Item>();
		if(!item->isPickable()) {
			*ad << "You cannot pick " << item->getName() << ". \n";
			return;
		}
		
		Inventory* inventory = ad->getAlive()
				->getBackpack()
				.assertType<Inventory>(GameStateInvalid::BackpackNotInventory)
				.unsafeCast<Inventory>();
		
		if (!inventory) {
			*ad << "You have no inventory to put " << item->getName() << " in. ";
			return;
		}
		
		if(inventory->getFreeWeight() < item->getWeight()) {
			*ad << "Content of " << inventory->getName() << " would be too heavy with " << item->getName() << ". ";
			*ad << item->getName() << " weights " << Utils::weightStr(item->getWeight()) 
					<< ", but there's only " << Utils::weightStr(inventory->getFreeWeight()) << " available.";
			return;
		}
		
		if(inventory->getFreeSpace() < item->getSize()) {
			*ad << "There is not enough space left for " << item->getName() << " in " << inventory->getName() << ". ";
			return;
		}
		// Everything is allright, let's add it
		Room* current = item->getSingleRelation(R_INSIDE, Relation::Slave, "The item is located in more than one room.")
							.safeCast<Room>();
		if(!current) {
			LOGS("PickupAction", Error) << "The item is nowhere?!" << LOGF;
			return;
		}
		ad->getGM()->removeRelation(current, item, R_INSIDE);
		inventory->addItem(item->getObjectPointer());
		*ad << "You've picked up " + item->getName() + ".";
	}
}