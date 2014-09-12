#include "Door.hpp"
#include "Location.hpp"
#include "../ObjectPointer.hpp"
#include "../ActionList.hpp"
#include "../RegexMatcher.hpp"
#include "../ActionDescriptor.hpp"
#include "../RandomString.hpp"
#include "../SentenceJoiner.hpp"

namespace Dungeon {
	
	Door::Door() {
	}

	void Door::getActions(ActionList* list, ObjectPointer callee) {
		LOGS("Door", Verbose) << "Getting actions on " << this->getId() << "." << LOGF;	
		IDescriptable::getActions(list, callee);
		// Add move actions to all rooms
		try{
			ObjectMap targets = this->getRelations(Relation::Master, R_TARGET);
			DoorwalkAction* action = new DoorwalkAction;
			for (auto& obj : targets) {
				if (obj.second != callee.safeCast<Alive>()->getLocation()) {
					action->addTarget(this);
					list->addAction(action);
					break;
				}
			}
		}
		catch (const std::out_of_range& e) {
			// What a weird door pointing nowhere...
		}
	}
	
	void DoorwalkAction::commitOnTarget(ActionDescriptor* ad, ObjectPointer door) {	
		door.assertType<Door>("How do you want to walk through door?");
		ObjectPointer target;
		try {
			ObjectMap targets = door->getRelations(Relation::Master, R_TARGET);
			for (auto& obj : targets) {
				if (obj.second != ad->getAlive()->getLocation()) {
					target = obj.second;
				}
			}
		}
		catch (const std::out_of_range& e) {
			// What a weird door pointing nowhere...
		}

		target.assertType<Location>("That seems to head nowhere. You've decided not to go there.");
		ad->getGM()->moveAlive(ad->getAlive(), target);
		door.unsafeCast<Door>()->onGoThrough(ad);
		target->triggerTraps("inside", ad);
		target.unsafeCast<Location>()->examine(ad);
	}
    
	string Door::getDescriptionSentence() {
		return getGroupDescriptionSentence({ this });
	}

	string Door::getGroupDescriptionSentence(vector<ObjectPointer> others) {
		SentenceJoiner doors;
		for (auto i = others.begin(); i != others.end(); i++) {
			doors << *i;
		}

		return RandomString::get()
				<< doors.getSentence("", "There is %. ", "There are %. ") << endr
				<< doors.getSentence("", "A % is casting a grimm shadow on the floor. ", "Around you are %. ") << endr
				<< doors.getSentence("", "You see the frame of a %. ", "You see %. ") << endr;
	}
	
	void Door::registerProperties(IPropertyStorage& storage) {
		storage.have(goThroughMessage, "door-gothrough", "Message that will be displayed when user goes through");
		IDescriptable::registerProperties(storage);
	}	

	void DoorwalkAction::explain(ActionDescriptor* ad) {
		*ad << "Use 'go to ...' to enter another room.";
	}
	
	bool DoorwalkAction::matchCommand(string command) {
		return RegexMatcher::match("(go to|cd) .+", command); // Common mistake :)
	}


	PERSISTENT_IMPLEMENTATION(Door)


}
