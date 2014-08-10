#include <stdlib.h>

#include "ActionDescriptor.hpp"

namespace Dungeon {

	ActionDescriptor::ActionDescriptor(Action* action, GameManager* gm, Alive* caller) : action(action), gm(gm), caller(caller) {
		id = rand();
	}

	Action* ActionDescriptor::getAction() {
		return this->action;
	}
	
	Alive* ActionDescriptor::getAlive() {
		return this->caller;
	}
	
	GameManager* ActionDescriptor::getGM() {
		return this->gm;
	}


}

