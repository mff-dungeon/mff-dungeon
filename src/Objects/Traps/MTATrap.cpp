#include <memory>

#include "MTATrap.hpp"
#include "../../Utils/SentenceJoiner.hpp"
#include "../../Actions/MultiTargetAction.hpp"
#include "../../Game/ObjectGroup.hpp"
#include "../../Game/ActionDescriptor.hpp"

namespace Dungeon {

	ObjectPointer MTATrap::wrapFind(ObjectMap objects, MultiTargetAction* action, string str, ActionDescriptor* ad) {
		bool any = false;
		try {
			ObjectGroup grp(objects);
			ObjectPointer target;
			string lower = Utils::decapitalize(string(str));
			
			size_t found = 0;
			if(lower.find("any") == 0) {
				found = str.find(" ");
				any = true;
			}
			if(found == string::npos || found == str.size()-1) {
				// There is nothing else than any[^ ]*, choose anything
				target = grp.begin()->second;
			}
			else {
				if(any) {
					str = str.substr(found+1);
				}
				target = grp.match(str);
			}
			IDescriptable* obj = target.safeCast<IDescriptable>();
			if (obj) {
				if(any) {
					*ad << "I've chosen " << obj->getName() << " for you." << eos;
				}
				LOGS("MTATrap", Debug) << "Selected " << obj->getLongName() << LOGF;
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

	bool MTATrap::exceptionTrigger(ActionDescriptor* ad) {
		switch (phase) {
			case Selecting:
				ad->waitForReply([this] (ActionDescriptor* ad, string reply) {
					if(Utils::decapitalize(string(reply)) == "none") { 
						// using copied string, to prevent modifying the entry
						*ad << "Okay, I won't do anything." << eos;
						phase = Cancel;
						throw TrapException(this);
					}
					target = wrapFind(this->objects, (MultiTargetAction*) ad->getAction(), reply, ad);
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
		return false;
	}

	PERSISTENT_IMPLEMENTATION(MTATrap)

}

