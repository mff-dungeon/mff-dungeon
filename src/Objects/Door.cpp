#include "Door.hpp"
#include "../Utils/RegexMatcher.hpp"
#include "../Utils/RandomString.hpp"
#include "../Utils/SentenceJoiner.hpp"
#include "../Game/ObjectPointer.hpp"
#include "../Game/ActionList.hpp"
#include "../Game/ActionDescriptor.hpp"
#include "../Game/GameManager.hpp"
#include "Traps/DoorLock.hpp"
#include "Location.hpp"

namespace Dungeon {

	Door::Door() { }

	void Door::getActions(ActionDescriptor *ad) {
		LOGS(Debug) << "Getting actions on " << this->getId() << "." << LOGF;
		// Add move actions to all rooms
		try {
			const ObjectMap& targets = this->getRelations(Relation::Master, R_TARGET);
			DoorwalkAction* action = new DoorwalkAction;
			auto loc = ad->getCaller()->getLocation();
			for (auto& obj : targets) {
				if (obj.second != loc) {
					action->addTarget(this);
					ad->getActionList().addAction(action)
							->useActionFor(this, ad);
					break;
				}
			}
		} catch (const std::out_of_range& e) {
			// What a weird door pointing nowhere...
		}
	}

	void DoorwalkAction::commitOnTarget(ActionDescriptor* ad, ObjectPointer door) {
		door.assertType<Door>("How do you want to walk through door?");
		door->triggerTraps("doorwalk", ad);
		ObjectPointer target;
		try {
			const ObjectMap& targets = door->getRelations(Relation::Master, R_TARGET);
			for (auto& obj : targets) {
				if (obj.second != ad->getCaller()->getLocation()) {
					target = obj.second;
				}
			}
		} catch (const std::out_of_range& e) {
			// What a weird door pointing nowhere...
		}

		target.assertType<Location>("That seemed to head nowhere. You've decided not to go there.");
		ad->getGM()->moveAlive(ad->getCaller(), target);
		door.unsafeCast<Door>()->onGoThrough(ad);
		target->triggerTraps("inside", ad);
		target.unsafeCast<Location>()->examine(ad);
	}

	Door* Door::addLock(ObjectPointer lock) {
		lock.assertExists("The lock doesn't exist.").assertType<DoorLock>("You are locking a door with something strange.");
		this->attachTrap(lock, "doorwalk");
		return this;
	}

	Door* Door::removeLock(ObjectPointer lock) {
		lock.assertExists("The lock doesn't exist.").assertType<DoorLock>("You are unlocking a door with something strange.");
		this->detachTrap(lock, "doorwalk");
		return this;
	}

	string Door::getDescriptionSentence() {
		return getGroupDescriptionSentence({this});
	}

	string Door::getGroupDescriptionSentence(vector<ObjectPointer> others) {
		SentenceJoiner doors;
		for (auto i = others.begin(); i != others.end(); i++) {
			doors << *i;
		}

		return RandomString::get()
				<< doors.getSentence("", "There is %.", "There is %.") << endr
				<< doors.getSentence("", "A % is casting a grimm shadow on the floor.", "Around you is %.") << endr
				<< doors.getSentence("", "You see the frame of a %.", "You see %.") << endr;
	}

	void Door::registerProperties(IPropertyStorage& storage) {
		storage.have(goThroughMessage, "door-gothrough", "Message that will be displayed when user goes through");
		IDescriptable::registerProperties(storage);
	}

	void DoorwalkAction::explain(ActionDescriptor* ad) {
		*ad << "go to ... - enter another room." << eos;
	}

	bool DoorwalkAction::match(const string& command, ActionDescriptor* ad) {
		RegexMatcher::matches matches;
		if (RegexMatcher::match("(go( to)?|cd|walk through|enter)( (.+))?", command, matches)) {
			selectBestTarget(matches[4], ad);
			return true;
		}
		return false;
	}

	PERSISTENT_IMPLEMENTATION(Door)
}
