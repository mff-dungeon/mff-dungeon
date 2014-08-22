#include <cstring>

#include "Item.hpp"
#include "../Actions/CallbackAction.hpp"
#include "../ActionDescriptor.hpp"

namespace Dungeon {
	
	Item::Item() {
		
	}
	
	Item::Item(objId id) {
		this->setId(id);
	}

	Item::~Item() {

	}
	
	void Item::getActions(ActionList* list, IObject* callee) {
		LOGS("Item", Verbose) << "Getting actions on " << this->getName() << "." << LOGF;
		// Check if the callee hasn't this item and it lies in the current room
		// FIX ME - find better way to check, if we can cast callee - we need its location
		try{
			if(strcmp(callee->className().c_str(),"Alive") == 0 || strcmp(callee->className().c_str(), "Human") == 0) {
				ObjectMap objectsInv = getRelations(false).at(R_INVENTORY);
				if(objectsInv.find(callee->getId()) != objectsInv.end()) {
					//Ok, I have this item in my inventory, I can drop it then
					string match = "Drop " + this->getName();
					transform(match.begin(), match.end(), match.begin(), ::tolower);
					list->addAction(new CallbackAction("drop", "Drop " + this->getName(), 
						RegexMatcher::matcher(match),
						[this, callee] (ActionDescriptor * ad) {
						*ad << "You've dropped " + this->getName();
						ad->getGM()->removeRelation(callee, this, R_INVENTORY);
						ad->getGM()->createRelation(((Alive*) callee)->getLocation()->get(), this, R_INSIDE);
						}));
				}
			}
		} catch (const std::out_of_range& e) {
			
		}
		try {
			if(strcmp(callee->className().c_str(),"Alive") == 0 || strcmp(callee->className().c_str(), "Human") == 0) {
				ObjectMap objectsIns = getRelations(false).at(R_INSIDE);
				if(objectsIns.find(((Alive*)callee)->getLocation()->getId()) != objectsIns.end()) {
					//Ok, It lies here, let's pick it up
					string match = "(Pick up|Take) " + this->getName();
					transform(match.begin(), match.end(), match.begin(), ::tolower);
					list->addAction(new CallbackAction("pickup", "Pick up " + this->getName(),
						RegexMatcher::matcher(match),
						[this, callee] (ActionDescriptor * ad) {
							// Assuming that we are picking an item in current location
							ad->getGM()->removeRelation(((Alive*) callee)->getLocation()->get(), this, R_INSIDE);
							ad->getGM()->createRelation(callee, this, R_INVENTORY);
							*ad << "You've picked up " + this->getName();
						}));
				}
			}
		}
		catch (const std::out_of_range& e) {
			
		}
	}
	
	string Item::getDescriptionSentence() {
		rand_init(gen, dist, 0, 1);
        int rnd = rand_next(gen, dist);
        
        switch (rnd) {
            case 0:
				return "You see " + this->getName() + " lying on the ground.";
            case 1:
                return "There lies" + this->getName() + ".";
            default:
                return this->getName() + " is nearby.";
        }
	}
	
	string Item::getGroupDescriptionSentence(vector<IDescriptable*> others) {
		if (others.size() == 0) return "";
        else if (others.size() == 1) return getDescriptionSentence();
        
        string sentence;
        rand_init(gen, dist, 0, 1);
        int rnd = rand_next(gen, dist);
        
        switch (rnd) {
            case 0:
                sentence = "You see ";
                break;
            case 1:
                sentence = "There lies ";
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
                sentence += " on the ground.";
                break;
            default:
                sentence += ".";
                break;
        }
        
        return sentence;
	}
	
	void Item::serialize(Archiver& stream) {
		IDescriptable::serialize(stream);
	}

	PERSISTENT_IMPLEMENTATION(Item)
}
