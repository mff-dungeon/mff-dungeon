#include <stdlib.h>

#include "ActionDescriptor.hpp"

namespace Dungeon {

	ActionDescriptor::ActionDescriptor(Driver* driver) : driver(driver) {
		id = rand();
		gm = 0;
		caller = 0;
		action = 0;
	}
	
	ActionDescriptor::~ActionDescriptor() {
		if (action)
			delete action;
	}

	void ActionDescriptor::enqueued(GameManager* gm) {
		this->gm = gm;
	}

	void ActionDescriptor::assigned(Alive* alive) {
		this->caller = alive;
	}

	void ActionDescriptor::matched(Action* action) {
		setAction(action);
	}

	void ActionDescriptor::setAction(Action* action) {
		if (this->action)
			delete this->action;
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
	
	ActionDescriptor& ActionDescriptor::operator<< (const string& msg){
		messages << msg;
		return *this;
	}
	
	ActionDescriptor& ActionDescriptor::operator<< (char const* msg){
		messages << msg;
		return *this;
	}
	
	ActionDescriptor& ActionDescriptor::operator<< (const int& msg){
		messages << msg;
		return *this;
	}


	TextActionDescriptor::TextActionDescriptor(Driver* driver) : ActionDescriptor(driver) {}

	string TextActionDescriptor::getReply() {
		return messages.str();
	}
	
	void TextActionDescriptor::clearReply() {
		messages.str("");
		messages.clear();
	}


}

