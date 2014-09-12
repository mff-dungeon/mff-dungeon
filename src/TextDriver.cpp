#include "TextDriver.hpp"
#include "ActionDescriptor.hpp"
#include "RandomString.hpp"
#include "Traps/Trap.hpp"

namespace Dungeon {
    
    TextDriver::TextDriver(ActionQueue* queue) : Driver(queue) {
    }

    TextDriver::~TextDriver() { 
    }
    
    bool TextDriver::process(TextActionDescriptor* ad) {
		try {
			if (!ad->isFinished()) {
				ad->userReplied(ad->in_msg);
				return true;
			}
			
			ad->getAlive()->getAllActions(&alist);

			string message (ad->in_msg);
			transform(message.begin(), message.end(), message.begin(), ::tolower);
			
			ofstream debugfile;
			debugfile.open("debug/messages.txt", ios::out | ios::app);
			debugfile << message << ";";
			
			for (ActionList::iterator it = alist.begin(); it != alist.end(); ++it) {
				Action* action = it->second;
				LOGS("TD", Verbose) << "Matching action " << it->first << LOGF;
				if (action->matchCommand(message)) {
					ad->matched(action);
					debugfile << action->type << endl;
					debugfile.close();
					alist.erase(it);
					break;
				}
			}
			alist.clear();
			
			if (!ad->isValid(this)) {
				*ad << getDontUnderstandResponse(ad->in_msg);
				debugfile << "!!!!!" << endl;
				debugfile.close();
				return false;
			}
			
			try {
				ad->state = ActionDescriptor::RoundBegin;
				ad->getAlive()->onBeforeAction(ad);
				ad->getAlive()->triggerTraps("action-" + ad->getAction()->type, ad);

				ad->state = ActionDescriptor::Round;
				ad->getAction()->commit(ad);

				ad->state = ActionDescriptor::RoundEnd;
				ad->getAlive()->onAfterAction(ad);
				return true;
			} catch (TrapException& te) {
				ad->state = ActionDescriptor::Trap;
				te.getTrap().unsafeCast<Trap>()->exceptionTrigger(ad);
			}
			
			while (ad->isValid(this)) {
				try {
					ad->getAction()->commit(ad);
					return true;
				} catch (TrapException& te) {
					te.getTrap().unsafeCast<Trap>()->exceptionTrigger(ad);
				}
			}
		}
		catch (GameException& gameException) {
			LOGS("Driver", Error) << gameException.what() << LOGF;
			if (ad->getAction() && ad->getAction()->handleException(gameException, ad))
				return false;
			*ad << gameException.what();
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