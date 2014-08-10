#include "TextDriver.hpp"
#include "ActionDescriptor.hpp"

namespace Dungeon {
    
    TextDriver::TextDriver(ActionQueue* queue) : Driver(queue) {
        this->alist = new ActionList();
    }

    TextDriver::~TextDriver() {
		for (size_t i = 0; i < alist->size(); i++) {
            delete alist->at(i);
		}
        
		delete alist;
    }
    
    bool TextDriver::process(TextActionDescriptor* ad) {
        alist->clear();
       
        ad->getAlive()->getAllActions(alist);
		
        for (Action* action : *alist) {
            if (action->matchCommand(ad->in_msg)) {
				ad->matched(action);
				break;
            }
        }
		
		if (ad->isValid(this)) {
			ad->getAction()->commit(ad);
			
			// TODO - process results, create nice reply
			return true;
		} else {
			ad->addMessage(getDontUnderstandResponse(ad->in_msg));
		}
        
        return false;
    }
    
    string TextDriver::getDontUnderstandResponse(string input) {
        stringstream ss;
		static 
        
        // TODO: think of something more creative
        
        int rnd = rand() % 6;
        switch (rnd) {
            case 0:
                ss << "What do you mean \"" << input << "\"?";
                break;
            case 1:
                ss << "The day may come when I understand \"" << input << "\" but it's not this day.";
                break;
            case 2:
                ss << "You just don't get it, do you? I do not understand \"" << input << "\"!";
                break;
            case 3:
                ss << "One does not simply ask me to \"" << input << "\".";
                break;
            case 4:
                ss << "I wouldn't do that if I were you. Besides, I can't do that.";
                break;
			case 5:
				ss << "This is not the thing you are trying to do.";
        }
        
        return ss.str();
    }
}