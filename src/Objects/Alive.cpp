#include <stdexcept>
#include "Alive.hpp"
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
			list->addAction(new CallbackAction("suicide", "commit suicide - If you just dont want to live on this planet anymore.",
				RegexMatcher::matcher("commit( a)? suicide|kill me( now)?"),
				[this] (ActionDescriptor * ad) {
						*ad << "You've killed yaself. Cool yeah?";
						this->hitpoints = 0;
				}));

			list->addAction(new CallbackAction("dump", "dump - If you want to get some info...",
				RegexMatcher::matcher("dump( relations)?"),
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

			list->addAction(new CallbackAction("help", "help - Well...",
				RegexMatcher::matcher("help|what can i do|list actions"),
				[this] (ActionDescriptor * ad) {
						*ad << "This is an non-interactive help. You can do following actions:\n";
						ActionList list;
						this->getAllActions(&list);
						for (auto& a : list) {
                            if (!a.second->isVisibleInHelp) continue;
							a.second->explain(ad);
							*ad << "\n";
						}
				}, false));

			list->addAction(new CallbackAction("explore", "explore - List items you can see in your current location",
				RegexMatcher::matcher("explore|where (the fuck )?am i|locate me|tell me where (the fuck )?am i|tell me my location"),
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
            
            list->addAction(new CallbackAction("hello", "hello - When you wanna be polite to your Dungeon",
               RegexMatcher::matcher("hello|hi|whats up|wazzup|yo"),
               [this] (ActionDescriptor * ad) {
                   *ad << "Hi!";
               }, false));
            
            list->addAction(new CallbackAction("who am i", "who am i - In case you forget your identity",
				RegexMatcher::matcher("who( )?am( )?i"),
				[this] (ActionDescriptor * ad) {
					*ad << "You are " + ad->getAlive()->getName() + ".";
				}, false));
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

	ObjectPointer* Alive::getLocation() {
		return getRelations(false).at(R_INSIDE).begin()->second;
	}


	PERSISTENT_IMPLEMENTATION(Alive)
}

