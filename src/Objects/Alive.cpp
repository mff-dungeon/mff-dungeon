#include <stdexcept>
#include "Alive.hpp"
#include "Room.hpp"
#include "../ActionDescriptor.hpp"
#include "../ActionList.hpp"

namespace Dungeon {
    
	Alive::Alive(objId id) : IDescriptable(id) {
		this->hitpoints = 1000;
	}
	
    void Alive::getAllActions(ActionList* list) {
		LOGS("Alive", Verbose) << "Getting all actions on " << this->getId() << "." << LOGF;
        // Add some actions on myself
        this->getActions(list, this);
		
		// Get actions for the inventory items
		LOGS("Alive", Verbose) << "Getting actions on inventory - " << this->getId() << "." << LOGF;
		try{
			ObjectMap inventory = getRelations(true).at("inventory");
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
			ObjectMap room = getRelations(false).at("inside");
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
	
	void Alive::serialize(Archiver& stream) {	// Implementation of serialization
		if(stream.isStoring()) {
			stream << hitpoints;
		}
		else {
			stream >> hitpoints;
		}
	}
    
    string Alive::getDescriptionSentence() {
        rand_init(gen, dist, 0, 3);
        int rnd = rand_next(gen, dist);
        
        switch (rnd) {
            case 0:
                return "You recognize " + this->getName() + "'s figure.";
            case 1:
                return this->getName() + " is there with you.";
            case 2:
                return "You smell the presence of " + this->getName() + ".";
            case 3:
            default:
                return this->getName() + " is nearby.";
        }
    }
    
    string Alive::getGroupDescriptionSentence(vector<IDescriptable *> others) {
        if (others.size() == 0) return "";
        else if (others.size() == 1) return getDescriptionSentence();
        
        string sentence;
        rand_init(gen, dist, 0, 3);
        int rnd = rand_next(gen, dist);
        
        switch (rnd) {
            case 0:
                sentence = "You recognize ";
                break;
            case 2:
                sentence = "You smell the presence of ";
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
            case 1:
                sentence += " are there with you.";
                break;
            case 3:
                sentence += " are nearby.";
                break;
            default:
                sentence += ".";
                break;
        }
        
        return sentence;
    }

	ObjectPointer Alive::getLocation() {
		return getRelations(false).at(R_INSIDE).begin()->second;
	}


	PERSISTENT_IMPLEMENTATION(Alive)
}

