#include <memory>
#include "MTATrap.hpp"
#include "../../Game/ActionDescriptor.hpp"
#include "../../Actions/MultiTargetAction.hpp"

namespace Dungeon {

	ObjectPointer MTATrap::wrapFind(ObjectMap objects, ActionDescriptor* ad, const string& str) {
		if (!ad)
			throw GameException("MTATrap::wrapFind was called without AD.");
		MultiTargetAction* action = dynamic_cast<MultiTargetAction*>(ad->getAction());
		if (!action)
			throw GameException("MTATrap::wrapFind was called when there was non-MTA action.");
		if (objects.size() == 0)
			return ObjectPointer();
		
		Info& info = action->mtatrap_info;
		Messages& msgs = action->messages;
		bool any = false;
		smatch matches;
		string lower = Utils::decapitalize(str);
		ObjectGroup grp(objects);
		SentenceJoiner targets;
		targets.setConjunction(", ", " or ");
		
		/* There are many objects, and the search string is empty.
		 * Ask user to choose one.
		 */
		if (str.length() == 0 && grp.size() > 1) {
			LOGS(Debug) << "No information provided - case 1." << LOGF;
			for (auto& pair : objects)
				targets << pair.second;
			*ad << targets.getSentence("", msgs.unspecified) << eos;
			info.objects = move(objects);
			info.phase = Selecting;
			throw TrapException(this);
		}
		
		/* The search string starts with any. Focus on what follows.
		 */
		if (regex_match(lower, matches, regex(msgs.anyRegex))) {
			LOGS(Debug) << "Entering 'any' mode." << LOGF;
			any = true;
			lower = matches[3];
		}
		
		try {
			LOGS(Debug) << "Regularly matching." << LOGF;
			return grp.match(str);
		} catch (const StringMatcher::Uncertain<ObjectPointer>& e) {
			LOGS(Debug) << "Uncertain exception." << LOGF;
			
			/* Search string started with any, but something matched better.
			 * Just choose any of remaining targets.
			 */
			if (any) {
				LOGS(Debug) << "Uncertain but any - case 2." << LOGF;
				ObjectPointer obj = e.possibleTargets.front();
				*ad << Utils::formatMessage(msgs.dontCare, obj) << eos;
				return obj;
			}

			info.objects.clear();
			for (ObjectPointer obj : e.possibleTargets)
				info.objects.insert(pair<objId, ObjectPointer>(obj.getId(),move(obj)));

			for (auto& pair : info.objects)
				targets << pair.second;

			info.phase = Selecting;
			*ad << targets.getSentence("", msgs.uncertain) << eos;
			throw TrapException(this);
		} catch (const StringMatcher::NoCandidate& e) {
			LOGS(Debug) << "No candidate exception." << LOGF;
			
			/* Search string started with any, but nothing matched.
			 * Just choose any target.
			 */
			if (any) {
				LOGS(Debug) << "No candidate in 'any' mode - case 3." << LOGF;
				ObjectPointer obj = objects.begin()->second;
				*ad << Utils::formatMessage(msgs.dontCare, obj) << eos;
				return obj;
			}
			
			/* There was only one choice, and didn't match.
			 * Check searchstring to be something like "it" or "that", 
			 * or to be empty (in that case warn user to be more specific),
			 * and select that one target.
			 */
			if (grp.size() == 1) {
				bool itMatched = regex_match(lower, matches, regex(msgs.itRegex));
				if (itMatched || lower.length() == 0) {
					LOGS(Debug) << "One (insufficient) candidate but 'it' mode - case 4." << LOGF;
					if (!itMatched) {
							*ad << msgs.nextTime << eos;
					}
					LOGS(Debug) << "Selected " << objects.begin()->first << " as the only candidate." << LOGF;
					return objects.begin()->second;
				}
			}
			
			LOGS(Debug) << "Giving up - case 5." << LOGF;
			/* Search string is not empty, not "any", nothing matched.
			 * Give up, we can't read minds.
			 */
			*ad << msgs.noCandidate << eos;
			info.phase = Cancel;
			throw TrapException(this);
		}
	}
	
	void MTATrap::exceptionTrigger(ActionDescriptor* ad) {
		MultiTargetAction* mta = static_cast<MultiTargetAction*>(ad->getAction());
		Info& info = mta->mtatrap_info;
		switch (info.phase) {
			case Selecting:
				ad->waitForReply([&info, this] (ActionDescriptor* ad, string reply) {
					if(Utils::decapitalize(reply) == "none"
							|| Utils::decapitalize(reply) == "nothing") {
						*ad << "Okay, I won't do anything." << eos;
						info.phase = Cancel;
						throw TrapException(this);
					}
					info.target = wrapFind(info.objects, ad, reply);
					info.phase = Return;
					throw TrapException(this);
				});
				ad->state = ActionDescriptor::Waiting;
				break;
			case Cancel:
				ad->state = ActionDescriptor::Finished;
				break;
			case Return:
				mta->setTarget(info.target);
				ad->state = ActionDescriptor::ActionReady;
				break;
		}
	}

	PERSISTENT_IMPLEMENTATION(MTATrap)

}

