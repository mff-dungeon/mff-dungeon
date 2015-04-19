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
		DoorwalkAction(const string& type = "door-walk") : MultiTargetAction(type) {
                    messages.unspecified = "Where to go? Either %.";
                    messages.nextTime = "There was just one option, but you should specify where to go next time.";
                    messages.dontCare = "If you don't care, I'll just follow %.";
                    messages.uncertain = "Sorry, did you want to follow %?";
                    messages.noCandidate = "Sorry, I don't understand where to go. Please try to explain it better.";
                    messages.itRegex = "it|there";
                    messages.anyRegex = "(any|some|wherever)(\\s+(.*))?;";
                    messages.anyRegexMatchIndex = 3;
                }

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

