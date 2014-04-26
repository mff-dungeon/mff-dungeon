#include <stdlib.h>

#include "ActionDescriptor.hpp"

namespace Dungeon {

	ActionDescriptor::ActionDescriptor(Action *action, GameManager *gm) : action(action), gm(gm) {
		id = rand();
	}

	Action* ActionDescriptor::getAction() {
		return this->action;
	}


}

