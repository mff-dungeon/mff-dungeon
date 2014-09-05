#include "Door.hpp"
#include "Room.hpp"
#include "../ObjectPointer.hpp"
#include "../ActionList.hpp"
#include "../RegexMatcher.hpp"
#include "../ActionDescriptor.hpp"
#include "../RandomString.hpp"

namespace Dungeon {
	
	Door::Door() {
	}

	void Door::getActions(ActionList* list, ObjectPointer callee) {
		LOGS("Door", Verbose) << "Getting actions on " << this->getId() << "." << LOGF;		
		// Add move actions to all rooms
		try{
			ObjectMap targets = this->getRelations(Relation::Master, R_TARGET);
			DoorwalkAction* action = new DoorwalkAction;
			for (auto& obj : targets) {
				if (obj.second != callee.safeCast<Alive>()->getLocation()) {
					action->addTarget(this->getObjectPointer());
					list->addAction(action);
					return;
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

		target.assertType<Room>("That seems to head nowhere. You've decided not to go there.");
		ad->getGM()->moveAlive(ad->getAlive(), target);
		target.unsafeCast<Room>()->explore(ad);
	}
    
    string Door::getDescriptionSentence() {
        return RandomString::get()
                << "There is a " + this->getName() + "." << endr
				<< "A " + this->getName() + " is casting a grimm shadow on the floor." << endr
				<< "You see the frame of a " + this->getName() + "." << endr;
    }
    
    string Door::getGroupDescriptionSentence(vector<ObjectPointer> others) {
        if (others.size() == 0) return "";
        else if (others.size() == 1) return getDescriptionSentence();
        
        string sentence;
		
        for (int i = 0; i < others.size() - 1; i++) {
            if (i != 0) sentence += ", ";
            sentence += others.at(i).safeCast<Door>()->getName();
        }
        
        sentence += " and " + others.back().safeCast<Door>()->getName();
		
        return RandomString::get()
				<< "There are " + sentence + "." << endr
				<< "Around you are " + sentence + "." << endr
				<< "You see " + sentence + "." << endr;
    }
	
	void DoorwalkAction::explain(ActionDescriptor* ad) {
		*ad << "Use 'go to ...' to enter another room.";
	}
	
	bool DoorwalkAction::matchCommand(string command) {
		return RegexMatcher::match("(go to|cd) .+", command); // Common mistake :)
	}


	PERSISTENT_IMPLEMENTATION(Door)


}
