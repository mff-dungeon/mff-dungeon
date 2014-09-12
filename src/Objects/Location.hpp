#ifndef LOCATION_HPP
#define	LOCATION_HPP

#include "../common.hpp"
#include "IDescriptable.hpp"
#include "../Actions/MultiTargetAction.hpp"

namespace Dungeon {
	
    /**
     * Technically not only room, but also any location.
     * Every alive must be placed into some room.
     * Room can be explored to see what's inside.
     */
    class Location : public IDescriptable {
    public:
        Location() {}		// Constructor allowing to load class later
        Location(objId id) : IDescriptable(id) {}
        virtual ~Location() {}

        virtual string getDescriptionSentence();
        virtual string getInsideSentence();
        virtual void getActions(ActionList* list, ObjectPointer calee);
        virtual void examine(ActionDescriptor* ad);

		virtual void registerProperties(IPropertyStorage& storage);

		bool contains(ObjectPointer object);
		bool isRespawnable() const;
		Location* setRespawnable(bool respawnable);
                string getEmptyMessage() const;
                Location* setEmptyMessage(string emptyMessage);

	private:
		bool respawnable = false;
                string emptyMessage = "";
	
    PERSISTENT_DECLARATION(Location, IDescriptable)
			
    };
	
	class PickupAction : public MultiTargetAction {
	public:
		PickupAction(string type = "location-pickup") : MultiTargetAction(type) {}
        
        virtual void explain(ActionDescriptor* ad);
        virtual bool matchCommand(string command);
        virtual void commitOnTarget(ActionDescriptor* ad, ObjectPointer target);

        virtual void commit(ActionDescriptor* ad);
	};
}

#endif	/* LOCATION_HPP */

