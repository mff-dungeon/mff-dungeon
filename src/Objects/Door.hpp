#ifndef DOOR_HPP
#define	DOOR_HPP

#include "../common.hpp"
#include "Virtual/IDescriptable.hpp"
#include "../Actions/MultiTargetAction.hpp"
#include "../Game/ActionDescriptor.hpp"

namespace Dungeon {

	/**
	 * Basic mean of movement.
	 */
	class Door : public IDescriptable {
	public:
		Door();
		Door(const objId& id) : IDescriptable(id) { }
		virtual void getActions(ActionDescriptor* ad);

		virtual string getDescriptionSentence();
		virtual string getGroupDescriptionSentence(vector<ObjectPointer> others);
		const string& getGoThroughMessage() const {
			return goThroughMessage;
		}
		Door* setGoThroughMessage(const string& goThroughMessage) {
			this->goThroughMessage = goThroughMessage;
			return this;
		}

		Door* addLock(ObjectPointer lock);
		Door* removeLock(ObjectPointer lock);

		virtual void registerProperties(IPropertyStorage& storage);
		virtual void onGoThrough(ActionDescriptor* ad) {
			*ad << goThroughMessage << eos;
			triggerTraps("go-through", ad);
		}

	private:
		string goThroughMessage = "";

		PERSISTENT_DECLARATION(Door, IDescriptable)
	};

	/**
	 * The multi-way teleporting action. Moves alive through.
	 * @param type
	 */
	class DoorwalkAction : public MultiTargetAction {
	public:
		DoorwalkAction(const string& type = "door-walk") : MultiTargetAction(type) { }

		virtual void explain(ActionDescriptor* ad);
		virtual bool match(const string& command, ActionDescriptor* ad);

		/**
		 * Will move the target to another Room.
		 * @param target Must be Door.
		 */
		virtual void commitOnTarget(ActionDescriptor* ad, ObjectPointer target);
	};
}

#endif	/* DOOR_HPP */

