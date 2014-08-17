#include "Door.hpp"
#include "../ObjectPointer.hpp"
#include "../Actions/CallbackAction.hpp"

namespace Dungeon {
	
	Door::Door() {
	}

	void Door::getActions(ActionList* list, IObject* callee) {
		LOGS("Door", Verbose) << "Getting actions on " << this->getId() << "." << LOGF;		
		// Add move actions to all rooms
		try{
			ObjectMap targets = this->getRelations(false).at(R_INSIDE);
			for (auto& obj : targets) {
				if (obj.first != callee->getId()) {
					list->push_back(new CallbackAction("door", "You can go through " + getName() + " with 'go to <room>'.", 
							RegexMatcher::matcher("go (through|to room)"),
							CALLBACK(Door, goThrough)));
				}
			}
		}
		catch (const std::out_of_range& e) {
			// What a weird door pointing nowhere...
		}
	}
	
	void Door::goThrough(ActionDescriptor* ad) {
		Room* target;
		ObjectMap targets = this->getRelations(false).at(R_INSIDE);
		for (auto& obj : targets) {
			// TODO: Implement object matching and use it (not only) here
			
			// FYI - this is NOT WORKING AS EXPECTED :)
			
			target = (Room*) obj.second->get();
		}
		
		ad->getGM()->moveAlive(ad->getAlive(), target->getId());
		*ad << "You've gone through " << getLongName() << ".\n";
		target->explore(ad);
	}
	
	PERSISTENT_IMPLEMENTATION(Door)


}
