#include "Alive.hpp"

namespace Dungeon {
    
    void Alive::getAllActions(ActionList* list) {
        // ask objects nearby for actions
        
        this->getActions(list, this);
    }
    
    void Alive::getActions(ActionList* list, IObject *callee) {
        if (callee == this) {
            // And add some actions "on myself"
            list->push_back(new CallbackAction("suicide", "If you just dont want to live on this planet anymore.",
                RegexMatcher::matcher("commit suicide"),
                [this] (ActionDescriptor * ad) {
                        ad->message = "You've killed yaself. Cool yeah?";
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
    
	PERSISTENT_IMPLEMENTATION(Alive)
}

