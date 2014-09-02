#include <stdexcept>
#include "Alive.hpp"
#include "Room.hpp"
#include "../ActionDescriptor.hpp"
#include "../ActionList.hpp"
#include "../RandomString.hpp"

namespace Dungeon {
    
	Alive::Alive(objId id) : IDescriptable(id) {
		this->hitpoints = 1000;
	}
	
    void Alive::getAllActions(ActionList* list) {
		LOGS("Alive", Verbose) << "Getting all actions on " << this->getId() << "." << LOGF;
        // Add some actions on myself
        this->getActions(list, this);
		
		// Get actions for the inventory items - now searches backpacks + thors hammer
		LOGS("Alive", Verbose) << "Getting actions on inventory - " << this->getId() << "." << LOGF;
		try{
			ObjectMap inventory = getRelations(true).at(R_INVENTORY);
			for(auto& item: inventory) {
				item.second.get()->getActions(list, this);
			}
		}
		catch (const std::out_of_range& e) {
			// Nothing needs to be done
		}
		
		LOGS("Alive", Verbose) << "Getting actions in location - " << this->getId() << "." << LOGF;
		// Find objects in current location
		try{
			ObjectMap room = getRelations(false).at(R_INSIDE);
			for(auto& item: room) {
				item.second.get()->getActions(list, this);
			}
		}
		catch (const std::out_of_range& e) {
			// Nothing needs to be done
		}
    }
    
    void Alive::getActions(ActionList* list, IObject *callee) {
		LOGS("Alive", Verbose) << "Getting actions on " << this->getId() << "." << LOGF;
		if (callee == this) {
			list->addAction(new CallbackAction("suicide", "commit suicide - If you just dont want to live on this planet anymore.",
				RegexMatcher::matcher("commit( a)? suicide|kill me( now)?"),
				[this] (ActionDescriptor * ad) {
						*ad << "You've killed yaself. Cool yeah?";
						this->hitpoints = 0;
				}));
		}
    }
	
	void Alive::registerProperties(IPropertyStorage& storage) {
		storage.have(hitpoints, "Current hitpoints");
		IDescriptable::registerProperties(storage);
	}

    
    string Alive::getDescriptionSentence() {
        return RandomString::get()
				<< "You recognize " + this->getName() + "'s figure." << endr
				<< this->getName() + " is there with you." << endr
				<< "You smell the presence of " + this->getName() + "." << endr
				<< this->getName() + " is nearby." << endr;
    }
    
    string Alive::getGroupDescriptionSentence(vector<IDescriptable *> others) {
        if (others.size() == 0) return "";
        else if (others.size() == 1) return getDescriptionSentence();
        
        string sentence;
        for (int i = 0; i < others.size() - 1; i++) {
            if (i != 0) sentence += ", ";
            sentence += others.at(i)->getName();
        }
        
        sentence += " and " + others.at(others.size() - 1)->getName();
		
        return RandomString::get()
				<< "You recognize " + sentence + "." << endr
				<< sentence + " are there with you." << endr
				<< "You smell the presence of " + sentence + "." << endr
				<< sentence + " are nearby." << endr;
    }

	ObjectPointer Alive::getLocation() {
		return getRelations(false).at(R_INSIDE).begin()->second;
	}


	PERSISTENT_IMPLEMENTATION(Alive)
}

