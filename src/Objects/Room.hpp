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
        Room() {}		// Constructor allowing to load class later
        Room(objId id) : IDescriptable(id) {}
        virtual ~Room() {}

        virtual string getDescriptionSentence();
        virtual void getActions(ActionList* list, ObjectPointer calee);
        virtual void explore(ActionDescriptor* ad);

		virtual void registerProperties(IPropertyStorage& storage);

		bool contains(ObjectPointer object);
		bool isRespawnable() const;
		Room* setRespawnable(bool respawnable);
		
	private:
		bool respawnable = true;
	
    PERSISTENT_DECLARATION(Room, IDescriptable)
			
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

