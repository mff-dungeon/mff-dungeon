#include "TextDriver.hpp"
#include "../Utils/RandomString.hpp"
#include "../Game/ActionDescriptor.hpp"
#include "../Objects/Traps/Trap.hpp"
#include "../Objects/Human.hpp"

namespace Dungeon {

	bool TextDriver::process(TextActionDescriptor* ad) {
		LOGS(Debug) << "Starting process cycle with AD in state " << ad->state << "." << LOGF;
		try {
			try {
				ad->getCaller()->markInteraction()->save();

				if (ad->state == ActionDescriptor::Waiting)
					processUserReply(ad);

				if (ad->state == ActionDescriptor::Empty)
					processFill(ad);

				if (ad->state == ActionDescriptor::ActionsFilled)
					processMatch(ad);

				if (ad->state == ActionDescriptor::ActionReady)
					processRun(ad);

			} catch (TrapException& te) {
				LOGS(Debug) << "Trap thrown." << LOGF;
				ad->state = ActionDescriptor::Trapped;
				te.getTrap().unsafeCast<Trap>()->exceptionTrigger(ad);
				
				// After trap we want to restart the process,
				// unless reply is awaited.
				if (ad->state != ActionDescriptor::Waiting)
					return process(ad); 
			}
		} catch (GameException& gameException) {
			resetPatience(ad->getCaller()->getId());
			LOGS(Error) << "Exception was thrown: " << gameException.what() << LOGF;
			ad->state = ActionDescriptor::Error;
			if (ad->getAction())
				ad->getAction()->handleException(gameException, ad);
			if (ad->state == ActionDescriptor::Error) {
				*ad << gameException.what() << eos;
				ad->state = ActionDescriptor::Finished;
			}
		}
		LOGS(Verbose) << "Finished processing action, user response: " << ad->getReply().plainString() << LOGF;
		return ad->state == ActionDescriptor::Finished;
	}

	void TextDriver::processFill(TextActionDescriptor *ad) {
		LOGS(Debug) << "Filling actions." << LOGF;
		ad->prepareFilling();
		ad->getCaller()->getAllActions(ad);
		ad->state = ActionDescriptor::ActionsFilled;
	}

	void TextDriver::processMatch(TextActionDescriptor *ad) {
		LOGS(Debug) << "Matching actions." << LOGF;
		string message (ad->in_msg);
		transform(message.begin(), message.end(), message.begin(), ::tolower);

		ofstream debugfile;
		debugfile.open("debug/messages.txt", ios::out | ios::app);
		debugfile << message << ";";

		ActionList& alist = ad->getActionList();
		for (ActionList::iterator it = alist.begin(); it != alist.end(); ++it) {
			Action* action = it->second;
			LOGS(Debug) << "Matching action " << it->first << LOGF;
			if (action->match(message, ad)) {
				ad->matched(action);
				LOGS(Verbose) << "Matched action " << it->first << LOGF;
				debugfile << action->type << endl;
				ad->state = ActionDescriptor::ActionReady;
				return;
			}
		}

		LOGS(Verbose) << "The query didn't match any possible action." << LOGF;
		*ad << getDontUnderstandResponse(ad->in_msg, getPatience(ad->getCaller()->getId())) << eos;
		ad->state = ActionDescriptor::Finished;
		debugfile << "??????" << endl;
		incrementPatience(ad->getCaller()->getId());
	}
	

	void TextDriver::processRun(TextActionDescriptor *ad) {
		LOGS(Debug) << "Running action " << ad->getAction()->type << LOGF;
		ad->getAction()->validate();
		resetPatience(ad->getCaller()->getId());
		ad->getCaller()->onBeforeAction(ad);

		ad->state = ActionDescriptor::ActionRunning;
		ad->getCaller()->triggerTraps("action-" + ad->getAction()->type, ad);
		ad->getAction()->commit(ad);

		ad->state = ad->isFinished() ? ActionDescriptor::ActionFinished : ActionDescriptor::Waiting;
		ad->getCaller()->onAfterAction(ad);
		ad->state = ad->isFinished() ? ActionDescriptor::Finished : ActionDescriptor::Waiting;
	}

	void TextDriver::processUserReply(TextActionDescriptor *ad) {
		LOGS(Debug) << "Processing reply." << LOGF;
		LOGS(Verbose) << "Action was already set in previous processing to " << ad->getAction()->type << ", using user dialog reply." << LOGF;
		ad->state = ActionDescriptor::ActionRunning;
		ad->userReplied(ad->in_msg);
		ad->state = ad->isFinished() ? ActionDescriptor::Finished : ActionDescriptor::Waiting;
	}

    string TextDriver::getDontUnderstandResponse(string input, int timeSinceLastMatched) {
        if (timeSinceLastMatched < 0)
            return "Impossibru.";
        else if (timeSinceLastMatched < 3)
            return RandomString::get()
                    << "Pardon me?" << endr
                    << "Come again?" << endr
                    << "I don't understand. Could you please rephrase your last message?" << endr
                    << "What do you mean \"" + input + "\"?" << endr;
        else if (timeSinceLastMatched < 10)
            return RandomString::get()
                    << "You just don't get it, do you? I don't understand \"" + input + "\"!" << endr
                    << "One does not simply ask me to \"" + input + "\"." << endr
                    << "You can try sending this one more time. Perhaps it will change something." << endr
                    << "This is not the thing you're trying to do." << endr
                    << "The day may come when I understand you but it's not this day." << endr
                    << "I wouldn't do that if I were you. Besides, I can't do that." << endr;
        else
            return RandomString::get()
                    << "I think I may have just failed my Turing test." << endr
                    << "Is there any way to convince you I don't know what you mean?" << endr
                    << "You must be really bored by now." << endr
                    << "Nope. Still don't know what you mean." << endr
                    << "Don't test my patience." << endr
                    << "You just lost the game." << endr;
    }
    
    Action* TextDriver::getCreateAction() {
        return new CallbackAction("create-user", "",
				RegexMatcher::matcher(".+"),
				[this] (ActionDescriptor * ad) {
					*ad << (RandomString::get()
							<< "Greetings, brave warrior! How may I call you?"  << endr
							<< "You have discovered the mighty Dungeon. What is thy name?" << endr
							<< "Beware of the great Dungeon! How do you wish to be called?" << endr)
							<< eos;
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
