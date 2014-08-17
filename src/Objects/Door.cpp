#include "Door.hpp"
#include "../ObjectPointer.hpp"
#include "../ActionList.hpp"

namespace Dungeon {
	
	Door::Door() {
	}

	void Door::getActions(ActionList* list, IObject* callee) {
		LOGS("Door", Verbose) << "Getting actions on " << this->getId() << "." << LOGF;		
		// Add move actions to all rooms
		try{
			ObjectMap targets = this->getRelations(false).at(R_INSIDE);
			DoorwalkAction* action = new DoorwalkAction;
			for (auto& obj : targets) {
				if (obj.first != callee->getId()) {
					action->addTarget(obj.second);
				}
			}
			list->addAction(action);
		}
		catch (const std::out_of_range& e) {
			// What a weird door pointing nowhere...
		}
	}
	
	void DoorwalkAction::commitOnTarget(ActionDescriptor* ad, ObjectPointer* target) {		
		ad->getGM()->moveAlive(ad->getAlive(), target->getId());
		((Room*) target->get())->explore(ad);
	}
    
    string Door::getDescriptionSentence() {
        rand_init(gen, dist, 0, 2);
        int rnd = rand_next(gen, dist);
        
        switch (rnd) {
            case 0:
                return "There is a " + this->getName() + ".";
            case 1:
                return "A " + this->getName() + " is casting a grimm shadow on the floor.";
            case 2:
            default:
                return "You see the frame of a " + this->getName() + ".";
        }
    }
	
	void DoorwalkAction::explain(ActionDescriptor* ad) {
		*ad << "Use 'go to ...' to enter another room. Currently you can go to:\n";
		for (auto& pair : targets) {
			IObject* obj = pair.second->get();
			if (obj->isDescriptable())
				*ad << "- " << ((IDescriptable*) obj)->getLongName() << "\n"; // Todo - output as sentence
		}
	}
	
	void DoorwalkAction::commit(ActionDescriptor* ad) {
		ObjectPointer* current = ad->getAlive()->getLocation();
		for (auto& pair : targets) {
			if (pair.second->getId() == current->getId()) continue;
			commitOnTarget(ad, pair.second); // TODO Implement object matching...
			return;
		}
		*ad << "No target good enough. Try again later.";
	}
	
	bool DoorwalkAction::matchCommand(string command) {
		return RegexMatcher::match("go to .+", command);
	}

	PERSISTENT_IMPLEMENTATION(Door)


}
