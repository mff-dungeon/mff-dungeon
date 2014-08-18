#include "Room.hpp"
#include "../ObjectPointer.hpp"

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
            TypeGroupedObjectMap groupedObjects(objects.begin(), objects.end());
            TypeGroupedObjectMap::iterator m_it, s_it;
            
            for (m_it = groupedObjects.begin(); m_it != groupedObjects.end(); m_it = s_it) {
                objId firstIdentifier = (*m_it).first;
                string objectType = objId_getType(firstIdentifier);
                
                if ((*m_it).second->get()->isDescriptable()) {
                    pair<TypeGroupedObjectMap::iterator, TypeGroupedObjectMap::iterator> keyRange = groupedObjects.equal_range(firstIdentifier);
                    vector<IDescriptable *> sameTypeObjects;
                    
                    for (s_it = keyRange.first; s_it != keyRange.second; s_it++) {
                        if ((*s_it).second->get() == ad->getAlive()) continue;
                        
                        sameTypeObjects.push_back((IDescriptable *)(*s_it).second->get());
                    }
                    
                    if (sameTypeObjects.empty()) {
                        continue;
                    } else if (sameTypeObjects.size() == 1) {
                        *ad << sameTypeObjects.front()->getDescriptionSentence() << " ";
                    } else {
                        *ad << sameTypeObjects.front()->getGroupDescriptionSentence(sameTypeObjects) << " ";
                    }
				} else {
					*ad << "There is an object (" << (*m_it).second->getId() << "). ";
				}
            }
		}
		catch (const std::out_of_range& e) {
		}
    }

	PERSISTENT_IMPLEMENTATION(Room)

}