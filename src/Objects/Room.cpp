/* 
 * File:   Room.cpp
 * Author: salmelu
 * 
 * Created on August 13, 2014, 3:05 PM
 */

#include "Room.hpp"

namespace Dungeon {

	Room::Room(objId id) {
		this->setId(id);
	}

	Room::~Room() {
		
	}
	
	void Room::getActions(ActionList* list, IObject *callee) {
		
    }
	
	void Room::serialize(Archiver& stream) {
		
	}
    
	PERSISTENT_IMPLEMENTATION(Room)

}