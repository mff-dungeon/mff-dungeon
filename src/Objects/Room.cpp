#include "Room.hpp"
#include "../ObjectPointer.hpp"
#include "../ObjectGroup.hpp"

namespace Dungeon {

	Room::Room(objId id) {
		this->setId(id);
	}

	Room::~Room() {
		
	}
	
	void Room::getActions(ActionList* list, IObject *callee) {
		LOGS("Room", Verbose) << "Getting actions on " << this->getName() << "." << LOGF;
		// Recursively search all items in this room
		try{
			ObjectMap objects = getRelations(true).at(R_INSIDE);
			for(auto& item: objects) {
				if (item.second->getId() != callee->getId())
					item.second->get()->getActions(list, callee);
			}
		}
		catch (const std::out_of_range& e) {
		}
    }
	
	void Room::explore(ActionDescriptor* ad) {
		LOGS("Room", Verbose) << "Exploring " << this->getName() << "." << LOGF;
		*ad << "You are in " << getLongName() << ". " << getDescription() << " ";
		// Recursively search all items in this room
		try{
			ObjectMap objects = getRelations(true).at(R_INSIDE);
            ObjectGroup groupedObjects(objects);
            
            // remove myself from the exploration group
            for (ObjectGroup::iterator it = groupedObjects.begin(); it != groupedObjects.end(); it++) {
                if (it->second->get() == ad->getAlive()) {
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

}