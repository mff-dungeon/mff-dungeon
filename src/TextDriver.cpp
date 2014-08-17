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
			*ad << getDontUnderstandResponse(ad->in_msg);
		}
        
        return false;
    }
    
    string TextDriver::getDontUnderstandResponse(string input) {
        // TODO: think of something more creative
        
        rand_init(gen, dist, 0, 5);
        int rnd = rand_next(gen, dist);
        switch (rnd) {
            case 0:
                return "What do you mean \"" + input + "\"?";
            case 1:
                return "The day may come when I understand \"" + input + "\" but it's not this day.";
            case 2:
                return "You just don't get it, do you? I do not understand \"" + input + "\"!";
            case 3:
                return "One does not simply ask me to \"" + input + "\".";
            case 4:
                return "I wouldn't do that if I were you. Besides, I can't do that.";
			case 5:
            default:
				return "This is not the thing you are trying to do.";
        }
    }
    
    string TextDriver::getStrangerResponse(string input) {
        stringstream ss;
		static
        
        // TODO: think of something more creative
        
        rand_init(gen, dist, 0, 2);
        int rnd = rand_next(gen, dist);
        switch (rnd) {
            case 0:
                return "The Dungeon speaks to no strangers. Add me as a friend and then we'll talk!";
            case 1:
                return "Who are you, puny human? Prove yourself to me and add me as a friend!";
            case 2:
            default:
                return "Do not disturb my peace, stranger. We can parley later upon you befriending me!";
        }
    }
    
    string TextDriver::getNewUserMessage() {
        stringstream ss;
		static
        
        // TODO: think of something more creative
        
        rand_init(gen, dist, 0, 2);
        int rnd = rand_next(gen, dist);
        switch (rnd) {
            case 0:
                return "Greetings, brave warrior! You may now begin your quest.";
            case 1:
                return "You have discovered the mighty Dungeon. Choose your words wisely!";
            case 2:
            default:
                return "Beware of the great Dungeon! Ask me a question and I shall answer you.";
        }
    }
}