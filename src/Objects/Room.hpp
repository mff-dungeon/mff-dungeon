#ifndef ROOM_HPP
#define	ROOM_HPP

#include "../common.hpp"
#include "IDescriptable.hpp"

namespace Dungeon {
	
    class Room : public IDescriptable {
    public:
        Room() {};		// Constructor allowing to load class later
        Room(objId id);
        virtual ~Room();

        virtual void getActions(ActionList* list, IObject* calee);
        virtual void explore(ActionDescriptor* ad);
	
    PERSISTENT_DECLARATION(Room, "Room")
			
    };
}

#endif	/* ROOM_HPP */

