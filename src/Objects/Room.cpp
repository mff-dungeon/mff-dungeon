#include <exception>

#include "Room.hpp"
#include "../ObjectPointer.hpp"
#include "../ObjectGroup.hpp"
#include "../ActionDescriptor.hpp"
#include "Item.hpp"
#include "Backpack.hpp"
#include "../RandomString.hpp"

namespace Dungeon {

	Room::Room(objId id) {
		this->setId(id);
	}

	Room::~Room() {
		
	}
	
	void Room::getActions(ActionList* list, IObject *callee) {
		LOGS("Room", Verbose) << "Getting actions on " << this->getName() << "." << LOGF;
		// Recursively search all items in this room
		try {
			ObjectMap objects = getRelations(true).at(R_INSIDE);
			for(auto& item: objects) {
				if (item.second.getId() != callee->getId())
					item.second.get()->getActions(list, callee);
			}
		}
		catch (const std::out_of_range& e) {
			
		}
		
		// Add pickup for items
		PickupAction* pickAction = new PickupAction;
		try {
			ObjectMap itemsIn = getRelations(true).at(R_INSIDE);
			for(auto& itemObj: itemsIn) {
				if(itemObj.second.get()->instanceOf(Item)) {
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
			ObjectMap objects = getRelations(true).at(R_INSIDE);
            ObjectGroup groupedObjects(objects);
            
            // remove myself from the exploration group
            for (ObjectGroup::iterator it = groupedObjects.begin(); it != groupedObjects.end(); it++) {
                if (it->second.get() == ad->getAlive()) {
                    groupedObjects.erase(it);
                    break;
                }
            }
            
            *ad << groupedObjects.explore();
		}
		catch (const std::out_of_range& e) {
			
		}
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
		// Presumes the target is an item, as it was checked when adding
		Item* item = (Item*) target.get();
		if(!item->isPickable()) {
			*ad << "You cannot pick " << item->getName() << ". \n";
			return;
		}
		
		// Let's get the backpack, supposes only one for now
		Backpack* backpack = 0;
		try {
			ObjectMap inv = ad->getAlive()->getRelations(true).at(R_INVENTORY);
			for(auto& item : inv) {
				if(item.second.get()->instanceOf(Backpack)) {
					backpack = (Backpack*) item.second.get();
				}
			}
		}
		catch (const std::out_of_range& e) {
			cout << "Fuck";
		}
		if(backpack == 0) {
			*ad << "You have no backpack to put " << item->getName() << " in. \n";
			return;
		}
		
		if(backpack->getFreeWeight() < item->getWeight()) {
			*ad << "Your backpack would be too heavy with " << item->getName() << ".";
			return;
		}
		
		if(backpack->getFreeSpace() < item->getSize()) {
			*ad << "There is no space left for " << item->getName() << " in your backpack. \n";
			return;
		}
		// Everything is allright, let's add it
		Room* current = 0;
		try {
			ObjectMap rooms = item->getRelations(false).at(R_INSIDE);
			if(rooms.size() != 1) {
				LOGS("PickupAction", Error) << "The item is nowhere?!" << LOGF;
				return;
			}
			for(auto& room : rooms) {
				current = (Room*) room.second.get();
			}
		}
		catch (const std::out_of_range& e) {
			
		}
		ad->getGM()->removeRelation(current, item, R_INSIDE);
		backpack->addItem(item);
		*ad << "You've picked up " + item->getName() + ".";
	}
}