#include <memory>

#include "MTATrap.hpp"
#include "../SentenceJoiner.hpp"
#include "../ActionDescriptor.hpp"
#include "../Actions/MultiTargetAction.hpp"

namespace Dungeon {
	
	ObjectPointer MTATrap::wrapFind(ObjectGroup group, MultiTargetAction* action, string str, ActionDescriptor* ad) {
		try {
			ObjectPointer target = group.match(str);
			IDescriptable* obj = target.safeCast<IDescriptable>();
			if (obj) LOGS("FMT", Verbose) << "Selected " << obj->getLongName() << LOGF;
			return target;
		} catch (const StringMatcher::Uncertain& e) {
			if (!ad)
				return ObjectPointer(); // Sorry, no chance
			
			ad->matched(action); // Because of thrown exception, this will be skipped
			this->group = group;
			this->phase = Selecting;
			
			if (e.possibleTargets.size() > 0) {
				SentenceJoiner targets;
				targets.setConjunction(", ", " or ");
				for (ObjectPointer obj : e.possibleTargets)
					targets << obj;
				
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
	
	bool MTATrap::exceptionTrigger(ActionDescriptor* ad) {
		switch (phase) {
			case Selecting:
				ad->waitForReply([this] (ActionDescriptor* ad, string reply) {
					target = wrapFind(group, (MultiTargetAction*) ad->getAction(), reply, ad);
					phase = Return;
					throw TrapException(this);
				});
			case Cancel:
				return false;
			case Return:
				MultiTargetAction* mta = (MultiTargetAction*) (ad->getAction());
				mta->setTarget(target);
				// If it's not MTA, why to call this at all?
				return true;
		}
	}
	
	
	PERSISTENT_IMPLEMENTATION(MTATrap)


}

