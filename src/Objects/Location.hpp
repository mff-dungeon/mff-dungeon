#ifndef LOCATION_HPP
#define	LOCATION_HPP

#include "../common.hpp"
#include "Virtual/IDescriptable.hpp"
#include "../Actions/MultiTargetAction.hpp"

namespace Dungeon {

	/**
	 * Technically not only room, but also any location.
	 * Every alive must be placed into some room.
	 * Room can be explored to see what's inside.
	 */
	class Location : public IDescriptable {
	public:
		Location() { } // Constructor allowing to load class later
		Location(const objId& id) : IDescriptable(id) { }
		virtual ~Location() { }

		virtual string getDescriptionSentence();
		virtual string getInsideSentence();
		virtual void getActions(ActionDescriptor* ad);
		virtual void examine(ActionDescriptor* ad);

		virtual void registerProperties(IPropertyStorage& storage);

		bool contains(ObjectPointer object);
		bool isRespawnable() const;
		Location* setRespawnable(bool respawnable);
		string getEmptyMessage() const;
		Location* setEmptyMessage(const string& emptyMessage);

	private:
		bool respawnable = false;
		string emptyMessage = "";

		PERSISTENT_DECLARATION(Location, IDescriptable)

	};

	class PickupAction : public MultiTargetAction {
	public:
		PickupAction(const string& type = "location-pickup") : MultiTargetAction(type) { }

		virtual void explain(ActionDescriptor* ad);
		virtual bool match(const string& command, ActionDescriptor* ad);
		virtual void commitOnTarget(ActionDescriptor* ad, ObjectPointer target);
	};
}

#endif	/* LOCATION_HPP */

