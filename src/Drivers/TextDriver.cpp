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
						
						/*
						 *					*** FIXME ***
						 *	Broken. ActionList is not cleared when a match is uncertain because of thrown TrapException.
						 *  Therefore, it leaves the list uncleared with old/invalid objects and next call of the affected 
						 *  action breaks the game.
						 *  Example: 
						 *		- drink potion
						 *		- which one, red or blue?
						 *		- red
						 *	( actions are left in the alist )
						 *		- drink potion
						 *		KABOOOOOOOOOOM
						 */
						
						for (ActionList::iterator it = alist.begin(); it != alist.end(); ++it) {
							Action* action = it->second;
							LOGS("TextDriver", Debug) << "Matching action " << it->first << LOGF;
							if (action->match(message, ad)) {
								ad->matched(action);
								LOGS("TextDriver", Verbose) << "Matched action " << it->first << LOGF;
								debugfile << action->type << endl;
								debugfile.close();
								break;
							}
						}
						alist.clear();

						if (!ad->isValid(this)) {
							LOGS("TextDriver", Verbose) << "The query didn't match any possible action." << LOGF;
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
					LOGS("TextDriver", Verbose) << "Action was already set in previous processing to " << ad->getAction()->type << ", using user dialog reply." << LOGF;
					ad->state = ActionDescriptor::Round;
					ad->userReplied(ad->in_msg);
				}	
				LOGS("TextDriver", Verbose) << "Finished action, user response: " << ad->getReply() << LOGF;
				return true;
			} catch (TrapException& te) {
				ad->state = ActionDescriptor::Trap;
				if (!te.getTrap().unsafeCast<Trap>()->exceptionTrigger(ad)) {
					LOGS("TextDriver", Verbose) << "Finished trap processing, user response: " << ad->getReply() << LOGF;
					return true;
				}
			}
			
			while (ad->isValid(this)) {
				try {
					LOGS("TextDriver", Verbose) << "Action was already set in previous processing to " << ad->getAction()->type << LOGF;
					ad->getAction()->commit(ad);
					LOGS("TextDriver", Verbose) << "Finished action, user response: " << ad->getReply() << LOGF;
					return true;
				} catch (TrapException& te) {
					if (!te.getTrap().unsafeCast<Trap>()->exceptionTrigger(ad)) {
						LOGS("TextDriver", Verbose) << "Finished trap processing, user response: " << ad->getReply() << LOGF;
						return true;
					}
				}
			}
		}
		catch (GameException& gameException) {
			LOGS("TextDriver", Error) << "Exception was thrown: " << gameException.what() << LOGF;
			if (ad->getAction() && ad->getAction()->handleException(gameException, ad))
				return false;
			*ad << gameException.what() << eos;
		}
        LOGS("TextDriver", Warning) << "Somehow, nothing happened at TextDriver processing." << LOGF;
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
