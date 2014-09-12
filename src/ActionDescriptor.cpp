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
        
        // courtesy of http://stackoverflow.com/questions/1798112/removing-leading-and-trailing-spaces-from-a-string
	
        // trim from left
        inline std::string& ltrim(std::string& s, const char* t = " \t\n\r\f") {
            s.erase(0, s.find_first_not_of(t));
            return s;
        }

        // trim from right
        inline std::string& rtrim(std::string& s, const char* t = " \t\n\r\f") {
            s.erase(s.find_last_not_of(t) + 1);
            return s;
        }

        // trim from left & right
        inline std::string& trim(std::string& s, const char* t = " \t\n\r\f") {
            return ltrim(rtrim(s, t), t);
        }
        
	void ActionDescriptor::addSentence(string msg) {
            string trimmed = trim(msg);
            
            if (trimmed == "") return;
            
            if (sentences++ > 1) {
                message += "\n";
            }
            
            sentences++;
            message += "  - ";            
            message += trimmed;
	}
	
	ActionDescriptor& ActionDescriptor::operator<< (const string& msg){
		currentSentence << msg;
		return *this;
	}
	
	ActionDescriptor& ActionDescriptor::operator<< (char const* msg){
		currentSentence << msg;
		return *this;
	}
	
	ActionDescriptor& ActionDescriptor::operator<< (const int& msg){
		currentSentence << msg;
		return *this;
	}
        
        ActionDescriptor::EndOfSentence *eos() {
            throw "Method eos is not callable.";
        }
        
        ActionDescriptor& ActionDescriptor::operator <<(ActionDescriptor::EndOfSentence*(*endofsentence)()) {
            this->addSentence(currentSentence.str());
            currentSentence.str("");
            return *this;
        }


	TextActionDescriptor::TextActionDescriptor(Driver* driver) : ActionDescriptor(driver) {}

	string TextActionDescriptor::getReply() {
            return message;
	}
	
	void TextActionDescriptor::clearReply() {
            message = "";
	}


}

