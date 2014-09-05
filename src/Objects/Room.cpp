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
		// If it throws an error, it's because there is nothing, so it should be safe to return false
		try {
			ObjectMap inside = getRelations(Relation::Master).at(R_INSIDE);
			if(inside.find(object->getId()) != inside.end())
				return true;
		}
		catch (const std::out_of_range& e) {
			
		}
		return false;
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
		Item* item = target.assertType<Item>("You can pick up only an item.").safeCast<Item>();
		if(!item->isPickable()) {
			*ad << "You cannot pick " << item->getName() << ". \n";
			return;
		}
		
		// Let's get the backpack, supposes only one for now
		Inventory* inventory = 0;
		try {
			ObjectMap inv = ad->getAlive()->getRelations(Relation::Master, Wearable::SlotRelations[Wearable::Slot::Backpack]);
			inventory = inv.begin()->second.safeCast<Inventory>();
		}
		catch (const std::out_of_range& e) {
			
		}
		if(inventory == 0) {
			*ad << "You have no inventory to put " << item->getName() << " in. ";
			return;
		}
		
		if(inventory->getFreeWeight() < item->getWeight()) {
			*ad << "Your inventory would be too heavy with " << item->getName() << ". ";
			return;
		}
		
		if(inventory->getFreeSpace() < item->getSize()) {
			*ad << "There is no space left for " << item->getName() << " in your inventory. ";
			return;
		}
		// Everything is allright, let's add it
		Room* current = 0;
		try {
			ObjectMap rooms = item->getRelations(Relation::Slave, R_INSIDE);
			if(rooms.size() != 1) {
				LOGS("PickupAction", Error) << "The item is nowhere?!" << LOGF;
				return;
			}
			current = rooms.begin()->second.safeCast<Room>();
		}
		catch (const std::out_of_range& e) {
			
		}
		ad->getGM()->removeRelation(current, item, R_INSIDE);
		inventory->addItem(item);
		*ad << "You've picked up " + item->getName() + ".";
	}
}