#include "TextDriver.hpp"
#include "../Utils/RandomString.hpp"
#include "../Game/ActionDescriptor.hpp"
#include "../Objects/Traps/Trap.hpp"
#include "../Objects/Human.hpp"

namespace Dungeon {
    
    bool TextDriver::process(TextActionDescriptor* ad) {
		try { // Catching regular exceptions
			try { // TrapExceptions to modify life-cycle
				if (ad->isFinished()) {
					if (!ad->getAction()) {
						ad->getCaller()->getAllActions(&alist);

						string message (ad->in_msg);
						transform(message.begin(), message.end(), message.begin(), ::tolower);

						ofstream debugfile;
						debugfile.open("debug/messages.txt", ios::out | ios::app);
						debugfile << message << ";";
						
						for (ActionList::iterator it = alist.begin(); it != alist.end(); ++it) {
							Action* action = it->second;
							LOGS("TextDriver", Verbose) << "Matching action " << it->first << LOGF;
							if (action->match(message, ad)) {
								ad->matched(action);
								debugfile << action->type << endl;
								debugfile.close();
								break;
							}
						}
						alist.clear();

						if (!ad->isValid(this)) {
							*ad << getDontUnderstandResponse(ad->in_msg) << eos;
							debugfile << "!!!!!" << endl;
							debugfile.close();
							return false;
						}
					}

					ad->getAction()->validate();

					ad->state = ActionDescriptor::RoundBegin;
					ad->getCaller()->onBeforeAction(ad);
					ad->getCaller()->triggerTraps("action-" + ad->getAction()->type, ad);

					ad->state = ActionDescriptor::Round;
					ad->getAction()->commit(ad);

					ad->state = ActionDescriptor::RoundEnd;
					ad->getCaller()->onAfterAction(ad);
				} else { // ! finished
					ad->state = ActionDescriptor::Round;
					ad->userReplied(ad->in_msg);
				}	
				return true;
			} catch (TrapException& te) {
				ad->state = ActionDescriptor::Trap;
				if (!te.getTrap().unsafeCast<Trap>()->exceptionTrigger(ad))
					return true;
			}
			
			while (ad->isValid(this)) {
				try {
					ad->getAction()->commit(ad);
					return true;
				} catch (TrapException& te) {
					if (!te.getTrap().unsafeCast<Trap>()->exceptionTrigger(ad))
						return true;
				}
			}
		}
		catch (GameException& gameException) {
			LOGS("TextDriver", Error) << gameException.what() << LOGF;
			if (ad->getAction() && ad->getAction()->handleException(gameException, ad))
				return false;
			*ad << gameException.what() << eos;
		}
        
        return false;
    }
    
    string TextDriver::getDontUnderstandResponse(string input) {
        return RandomString::get()
                << "What do you mean \"" + input + "\"?" << endr
                << "The day may come when I understand \"" + input + "\" but it's not this day." << endr
                << "You just don't get it, do you? I do not understand \"" + input + "\"!" << endr
                << "One does not simply ask me to \"" + input + "\"." << endr
                << "I wouldn't do that if I were you. Besides, I can't do that." << endr
				<< "This is not the thing you are trying to do." << endr;
    }
    
    Action* TextDriver::getCreateAction() {
        return new CallbackAction("create-user", "",
				RegexMatcher::matcher(".+"),
				[this] (ActionDescriptor * ad) {
					*ad << (RandomString::get()
                                                << "Greetings, brave warrior! How may I call you?"  << endr
                                                << "You have discovered the mighty Dungeon. What is thy name?" << endr
                                                << "Beware of the great Dungeon! How do you wish to be called?" << endr) << eos;
					ad->waitForReply([] (ActionDescriptor *ad, string reply) {
						((Human*) ad->getCaller())->setUsername(reply)
								->save();
						*ad << "Welcome, " << ad->getCaller()->getName() << "!" << eos; // A common mistake
						*ad << "You may begin your quest. Ask me and I shall answer you." << eos;
					});
					
					// TODO: add other first time questions
				}, false);
    }
}
