#include "ActionDescriptor.hpp"

namespace Dungeon {

	ActionDescriptor::ActionDescriptor(Action *action, GameManager *gm) : action(action), gm(gm) {
	}

	Action* ActionDescriptor::getAction() {
		return this->action;
	}


}

