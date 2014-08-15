#include "Room.hpp"
#include "../ObjectPointer.hpp"

namespace Dungeon {

	Room::Room(objId id) {
		this->setId(id);
	}

	Room::~Room() {
		
	}
	
	void Room::getActions(ActionList* list, IObject *callee) {
		LOGS("Room", Verbose) << "Getting actions on " << this->getId() << "." << LOGF;
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
	
	void Room::serialize(Archiver& stream) {
		
	}
    
	PERSISTENT_IMPLEMENTATION(Room)

}