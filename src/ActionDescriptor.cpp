#include <stdlib.h>

#include "ActionDescriptor.hpp"

namespace Dungeon {

	ActionDescriptor::ActionDescriptor(Driver* driver) : driver(driver) {
		id = rand();
		gm = 0;
		caller = 0;
		action = 0;
	}

	void ActionDescriptor::enqueued(GameManager* gm) {
		this->gm = gm;
	}

	void ActionDescriptor::assigned(Alive* alive) {
		this->caller = alive;
	}

	void ActionDescriptor::matched(Action* action) {
		this->action = action;
	}

	Action* ActionDescriptor::getAction() {
		return action;
	}

	Alive* ActionDescriptor::getAlive() {
		return caller;
	}

	GameManager* ActionDescriptor::getGM() {
		return gm;
	}

	bool ActionDescriptor::isValid(Driver* driver) {
		return gm && caller && action && driver == this->driver;
	}
	
	void ActionDescriptor::addMessage(string msg) {
		messages << msg;
	}

	TextActionDescriptor::TextActionDescriptor(Driver* driver) : ActionDescriptor(driver) {}

	string TextActionDescriptor::getReply() {
		return messages.str();
	}

}

