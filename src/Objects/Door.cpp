#include "Door.hpp"
#include "Room.hpp"
#include "../ObjectPointer.hpp"
#include "../ActionList.hpp"
#include "../RegexMatcher.hpp"
#include "../ActionDescriptor.hpp"

namespace Dungeon {
	
	Door::Door() {
	}

	void Door::getActions(ActionList* list, IObject* callee) {
		LOGS("Door", Verbose) << "Getting actions on " << this->getId() << "." << LOGF;		
		// Add move actions to all rooms
		try{
			ObjectMap targets = this->getRelations(true).at(R_TARGET);
			DoorwalkAction* action = new DoorwalkAction;
			for (auto& obj : targets) {
				if (obj.second != ((Alive*) callee)->getLocation()) {
					action->addTarget(this->getObjectPointer());
				}
			}
			list->addAction(action);
		}
		catch (const std::out_of_range& e) {
			// What a weird door pointing nowhere...
		}
	}
	
	void DoorwalkAction::commitOnTarget(ActionDescriptor* ad, ObjectPointer door) {	
			ObjectPointer target;
			try {
				ObjectMap targets = door.get()->getRelations(true).at(R_TARGET);
				for (auto& obj : targets) {
					if (obj.second != ad->getAlive()->getLocation()) {
						target = obj.second;
					}
				}
			}
			catch (const std::out_of_range& e) {
				// What a weird door pointing nowhere...
			}
			
            if (!target) {
                *ad << "That seems to head nowhere. You've decided not to go there.";
                return;
            }
            ad->getGM()->moveAlive(ad->getAlive(), target.getId());
            ((Room*) target.get())->explore(ad);
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
    
    string Door::getGroupDescriptionSentence(vector<IDescriptable *> others) {
        if (others.size() == 0) return "";
        else if (others.size() == 1) return getDescriptionSentence();
        
        string sentence;
        rand_init(gen, dist, 0, 2);
        int rnd = rand_next(gen, dist);
        
        switch (rnd) {
            case 0:
                sentence = "There are ";
                break;
            case 1:
                sentence = "Around you are ";
                break;
            case 2:
                sentence = "You see ";
                break;
            default:
                sentence = "";
                break;
        }
        
        for (int i = 0; i < others.size() - 1; i++) {
            if (i != 0) sentence += ", ";
            sentence += others.at(i)->getName();
        }
        
        sentence += " and " + others.at(others.size() - 1)->getName();
        
        switch (rnd) {
            default:
                sentence += ".";
                break;
        }
        
        return sentence;
    }
	
	void DoorwalkAction::explain(ActionDescriptor* ad) {
		*ad << "Use 'go to ...' to enter another room. Currently you can go to:\n";
		for (auto& pair : targets) {
			IObject* obj = pair.second.get();
			if (obj->instanceOf("IDescriptable"))
				*ad << "- " << ((IDescriptable*) obj)->getLongName() << "\n"; // Todo - output as sentence
		}
	}
	
	bool DoorwalkAction::matchCommand(string command) {
		return RegexMatcher::match("(go to|cd) .+", command); // Common mistake :)
	}


	PERSISTENT_IMPLEMENTATION(Door)


}
