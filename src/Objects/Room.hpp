#ifndef ROOM_HPP
#define	ROOM_HPP

#include "../common.hpp"

namespace Dungeon {
	
    class Room : public IObject{
    public:
            Room() {};		// Constructor allowing to load class later
            Room(objId id);
            virtual ~Room();

            virtual void getActions(ActionList* list, IObject* calee);

    protected:
            virtual void serialize(Archiver& stream);
	
    PERSISTENT_DECLARATION(Room, "Room")
			
    };
}

#endif	/* ROOM_HPP */

