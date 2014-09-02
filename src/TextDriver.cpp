#include "TextDriver.hpp"
#include "ActionDescriptor.hpp"
#include "RandomString.hpp"

namespace Dungeon {
    
    TextDriver::TextDriver(ActionQueue* queue) : Driver(queue) {
        this->alist = new ActionList();
    }

    TextDriver::~TextDriver() {        
		delete alist;
    }
    
    bool TextDriver::process(TextActionDescriptor* ad) {
		if (!ad->isFinished()) {
			ad->userReplied(ad->in_msg);
			return true;
		} 
		
        alist->clear();
        ad->getAlive()->getAllActions(alist);
		
		string message (ad->in_msg);
		// We don't care, if user writes "Potion" or "potion"
		transform(message.begin(), message.end(), message.begin(), ::tolower);
		
        for (auto& pair: *alist) {
			Action* action = pair.second;
			LOGS("TD", Verbose) << "Matching action " << pair.first << LOGF;
            if (action->matchCommand(message)) {
				ad->matched(action);
				break;
            }
        }
		
		if (ad->isValid(this)) {
			ad->getAction()->commit(ad);
			return true;
		} else {
			*ad << getDontUnderstandResponse(ad->in_msg);
		}
        
        return false;
    }
    
    string TextDriver::getDontUnderstandResponse(string input) {
        // TODO: think of something more creative
        return RandomString::get()
                << "What do you mean \"" + input + "\"?" << endr
                << "The day may come when I understand \"" + input + "\" but it's not this day." << endr
                << "You just don't get it, do you? I do not understand \"" + input + "\"!" << endr
                << "One does not simply ask me to \"" + input + "\"." << endr
                << "I wouldn't do that if I were you. Besides, I can't do that." << endr
				<< "This is not the thing you are trying to do." << endr;
    }
    
    string TextDriver::getStrangerResponse(string input) {
        // TODO: think of something more creative
        return RandomString::get()
                << "The Dungeon speaks to no strangers. Add me as a friend and then we'll talk!" << endr
				<< "Who are you, puny human? Prove yourself to me and add me as a friend!" << endr
				<< "Do not disturb my peace, stranger. We can parley later upon you befriending me!" << endr;
    }
    
    string TextDriver::getNewUserMessage() {
        // TODO: think of something more creative
        return RandomString::get()
				<< "Greetings, brave warrior! You may now begin your quest."  << endr
				<< "You have discovered the mighty Dungeon. Choose your words wisely!" << endr
                << "Beware of the great Dungeon! Ask me a question and I shall answer you." << endr;
    }
}