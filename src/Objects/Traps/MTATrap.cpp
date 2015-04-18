#include <memory>

#include "MTATrap.hpp"
#include "../../Utils/SentenceJoiner.hpp"
#include "../../Actions/MultiTargetAction.hpp"
#include "../../Game/ObjectGroup.hpp"
#include "../../Game/ActionDescriptor.hpp"
#include <regex>

namespace Dungeon {

	ObjectPointer MTATrap::wrapFind(ObjectMap objects, MultiTargetAction* action, const string& str, ActionDescriptor* ad) {
		bool any = false;
		try {
			ObjectGroup grp(objects);
			ObjectPointer target;
			string lower = Utils::decapitalize(str);

			smatch matches;
			if (regex_match(lower, matches, regex("any\\s+(.*)"))) {
				if (matches[1].length() > 0)
					target = grp.match(matches[1]);
				else
					target = grp.begin()->second;
			} else {
				target = grp.match(str);
			}

			IDescriptable* obj = target.safeCast<IDescriptable>();
			if (obj) {
				if(any) {
					*ad << "I've chosen " << obj->getName() << " for you." << eos;
				}
				LOGS(Debug) << "Selected " << obj->getLongName() << LOGF;
			}
			return target;
		} catch (const StringMatcher::Uncertain<ObjectPointer>& e) {
			if (!ad)
				return ObjectPointer(); // Sorry, no chance
			if (any) { // Starts with any, user doesn't care, return the first one
				if(e.possibleTargets.size() > 0) {
					// Unsafe cast is possible, in OG::match there are only IDescriptables added
					IDescriptable* obj = e.possibleTargets.at(0).unsafeCast<IDescriptable>();
					*ad << "If you don't care, I'll just take " << obj->getName() << "." << eos;
					return e.possibleTargets.at(0);
				}
			}

			ad->matched(action); // Because of thrown exception, this will be skipped
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

				*ad << "Sorry, did you mean " << targets << "?" << eos;
			} else {
				*ad << "Sorry, I don't know what you mean. Please try to explain it better." << eos;
			}

			throw TrapException(this);
		} catch (const StringMatcher::NoCandidate& e) {
			*ad << "No such thing found. Really." << eos;
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

