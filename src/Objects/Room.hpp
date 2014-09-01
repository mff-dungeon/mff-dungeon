#ifndef ROOM_HPP
#define	ROOM_HPP

#include "../common.hpp"
#include "IDescriptable.hpp"
#include "../Actions/MultiTargetAction.hpp"

namespace Dungeon {
	
    /**
     * Technically not only room, but also any location.
     * Every alive must be placed into some room.
     * Room can be explored to see what's inside.
     */
    class Room : public IDescriptable {
    public:
        Room() {};		// Constructor allowing to load class later
        Room(objId id);
        virtual ~Room();

        virtual string getDescriptionSentence();
        virtual void getActions(ActionList* list, IObject* calee);
        virtual void explore(ActionDescriptor* ad);
	
    PERSISTENT_DECLARATION(Room)
			
    };
	
	class PickupAction : public MultiTargetAction {
	public:
		PickupAction(string type = "room-pickup") : MultiTargetAction(type) {}
        
        virtual void explain(ActionDescriptor* ad);
        virtual bool matchCommand(string command);
        virtual void commitOnTarget(ActionDescriptor* ad, ObjectPointer target);

        virtual void commit(ActionDescriptor* ad);
	};
}

#endif	/* ROOM_HPP */

