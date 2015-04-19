#include <memory>
#include "MTATrap.hpp"
#include "../../Game/ActionDescriptor.hpp"
#include "../../Actions/MultiTargetAction.hpp"

namespace Dungeon {

	ObjectPointer MTATrap::wrapFind(ObjectMap objects, MultiTargetAction* action, const string& str, ActionDescriptor* ad) {
		using Messages = MultiTargetAction::DefaultMessages;
		bool any = false;
		string lower = Utils::decapitalize(str);
		smatch matches;
		
		try {
			ObjectGroup grp(objects);
			ObjectPointer target;

			if (str.length() == 0 && grp.size() > 1) {
				this->phase = Selecting;
				SentenceJoiner targets;
				targets.setConjunction(", ", " or ");
				for (auto& pair : objects)
					targets << pair.second;
				this->objects = move(objects);
				*ad << targets.getSentence(Messages::unspecifiedMore) << eos;
				throw TrapException(this);
			} else if (regex_match(lower, matches, regex("any\\s+(.*)"))) {
				if (matches[1].length() > 0)
					target = grp.match(matches[1]);
				else
					target = grp.begin()->second;
			} else {
				target = grp.match(str);
			}

			if (!!target) {
				if(any) {
					*ad << Utils::formatMessage(Messages::chosenForYou, target) << eos;
				}
				LOGS(Debug) << "Selected " << target << LOGF;
			}
			return target;
		} catch (const StringMatcher::Uncertain<ObjectPointer>& e) {
			if (!ad)
				return ObjectPointer(); // Sorry, no chance
			if (any) { // Starts with any, user doesn't care, return the first one
				if(e.possibleTargets.size() > 0) {
					// Unsafe cast is possible, in OG::match there are only IDescriptables added
					ObjectPointer obj = e.possibleTargets.at(0);
					*ad << Utils::formatMessage(Messages::dontCare, obj) << eos;
					return e.possibleTargets.at(0);
				}
			}

			this->phase = Selecting;

			if (e.possibleTargets.size() > 0) {
				SentenceJoiner targets;
				targets.setConjunction(", ", " or ");
				this->objects.clear();
				for (ObjectPointer obj : e.possibleTargets)
					this->objects.insert(pair<objId, ObjectPointer>(obj.getId(),obj));

				// This time there will be each object only once
				for (auto& pair : this->objects)
					targets << pair.second;

				*ad << targets.getSentence(Messages::uncertain) << eos;
			} else {
				*ad << Messages::totallyUncertain << eos;
			}

			throw TrapException(this);
		} catch (const StringMatcher::NoCandidate& e) {
			if (objects.size() == 1) {
				bool itMatched = regex_match(lower, matches, regex("it|that"));
				if (itMatched || lower.length() == 0) {
					if (!itMatched) {
							*ad << Messages::nextTime << eos;
					}
					LOGS(Debug) << "Selected " << objects.begin()->first << " as the only candidate." << LOGF;
					return objects.begin()->second;
				}
			}
			*ad << Messages::noCandidate << eos;
			phase = Cancel;
			throw TrapException(this);
		}
	}
	
	void MTATrap::exceptionTrigger(ActionDescriptor* ad) {
		switch (phase) {
			case Selecting:
				ad->waitForReply([this] (ActionDescriptor* ad, string reply) {
					if(Utils::decapitalize(reply) == "none"
							|| Utils::decapitalize(reply) == "nothing") {
						*ad << "Okay, I won't do anything." << eos;
						phase = Cancel;
						throw TrapException(this);
					}
					target = wrapFind(this->objects, (MultiTargetAction*) ad->getAction(), reply, ad);
					phase = Return;
					throw TrapException(this);
				});
				ad->state = ActionDescriptor::Waiting;
				break;
			case Cancel:
				ad->state = ActionDescriptor::Finished;
				break;
			case Return:
				MultiTargetAction* mta = (MultiTargetAction*) (ad->getAction());
				mta->setTarget(target);
				ad->state = ActionDescriptor::ActionReady;
				break;
		}
	}

	PERSISTENT_IMPLEMENTATION(MTATrap)

}

