#include <stdexcept>
#include "Alive.hpp"
#include "../ActionDescriptor.hpp"

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
				item.second->get()->getActions(list, this);
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
				item.second->get()->getActions(list, this);
			}
		}
		catch (const std::out_of_range& e) {
			// Nothing needs to be done
		}
    }
    
    void Alive::getActions(ActionList* list, IObject *callee) {
		LOGS("Alive", Verbose) << "Getting actions on " << this->getId() << "." << LOGF;
		
		if (callee == this) {
			list->push_back(new CallbackAction("suicide", "commit suicide - If you just dont want to live on this planet anymore.",
				RegexMatcher::matcher("commit( a)? suicide"),
				[this] (ActionDescriptor * ad) {
						*ad << "You've killed yaself. Cool yeah?";
						this->hitpoints = 0;
				}));

			list->push_back(new CallbackAction("dump", "dump - If you want to get some info...",
				RegexMatcher::matcher("dump"),
				[this] (ActionDescriptor * ad) {
						*ad << "So you want to know something? Relations which you master:\n";
						RelationList r = this->getRelations(true);
						for (auto& type : r) {
							*ad << type.first + ":\n";
							for(auto& obj : type.second) {
								*ad << "\t" + obj.first + "\n";
							}
						}
						*ad << "Slave:\n";
						r = this->getRelations(false);
						for (auto& type : r) {
							*ad << type.first + ":\n";
							for(auto& obj : type.second) {
								*ad << "\t" + obj.first + "\n";
							}
						}
				}));

			list->push_back(new CallbackAction("help", "help - Well...",
				RegexMatcher::matcher("help"),
				[this] (ActionDescriptor * ad) {
						*ad << "This is an non-interactive help. You can do following actions:\n";
						ActionList list;
						this->getAllActions(&list);
						for (auto& a : list) {
							a->explain(ad);
							*ad << "\n";
						}
				}));

			list->push_back(new CallbackAction("explore", "explore - List items you can see in your current location",
				RegexMatcher::matcher("explore"),
				[this] (ActionDescriptor * ad) {
						ObjectMap rooms = this->getRelations(false).at(R_INSIDE);
						for (auto& room : rooms) {
							IObject* obj = room.second->get();
							Room* r = (Room*) obj;
							if (r) {
								r->explore(ad);
							} else {
								*ad << "Non-room location: " << obj->getId() << "\n";
								ObjectMap objects = obj->getRelations(true).at(R_INSIDE);
								for(auto& o : objects) {
									*ad << "\t" + o.first + "\n";
								}
							}
						}
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


	PERSISTENT_IMPLEMENTATION(Alive)
}

