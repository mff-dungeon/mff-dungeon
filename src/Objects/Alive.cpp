#include <stdexcept>
#include "Alive.hpp"
#include "../ActionDescriptor.hpp"

namespace Dungeon {
    
	Alive::Alive(objId id) : IObject(id) {
		this->hitpoints = 1000;
	}
	
    void Alive::getAllActions(ActionList* list) {
        // Add some actions on myself
        this->getActions(list, this);
		
		// Get actions for the inventory items
		try{
			ObjectMap inventory = getRelations(true).at("inventory");
			for(auto& item: inventory) {
				item.second->get()->getActions(list, this);
			}
		}
		catch (const std::out_of_range& e) {
			// Nothing needs to be done
		}
    }
    
    void Alive::getActions(ActionList* list, IObject *callee) {
		list->push_back(new CallbackAction("suicide", "sucide - If you just dont want to live on this planet anymore.",
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
    }
	
	void Alive::serialize(Archiver& stream) {	// Implementation of serialization
		if(stream.isStoring()) {
			stream << hitpoints;
		}
		else {
			stream >> hitpoints;
		}
	}
    
	PERSISTENT_IMPLEMENTATION(Alive)
}

