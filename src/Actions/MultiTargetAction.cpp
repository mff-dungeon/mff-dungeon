#include "MultiTargetAction.hpp"
#include "UseAction.hpp"
#include "../Utils/FuzzyStringMatcher.hpp"
#include "../Game/ObjectPointer.hpp"
#include "../Game/ObjectGroup.hpp"
#include "../Game/ActionDescriptor.hpp"

namespace Dungeon {

	MultiTargetAction* MultiTargetAction::addTarget(ObjectPointer op) {
		if (targets.find(op.getId()) == targets.end())
			targets.insert(pair<objId, ObjectPointer>(op.getId(),op));
		return this;
	}

	void MultiTargetAction::merge(MultiTargetAction* second) {
		for (auto& pair : second->getTargets()) {
			addTarget(pair.second);
		}
	}

	const ObjectMap& MultiTargetAction::getTargets() const {
		return targets;
	}

	void MultiTargetAction::commit(ActionDescriptor* ad) {
		commitOnTarget(ad, selectedTarget);
	}

	void MultiTargetAction::selectBestTarget(const string& str, ActionDescriptor* ad) {
		if (!ad) throw GameException("MTA needs AD.");
		ad->matched(this); // Because of thrown exception, this will be skipped
		ObjectPointer trap = ad->getGM()->getObject("trap/mta");
		setTarget(trap.unsafeCast<MTATrap>()->wrapFind(targets, this, str, ad));
	}

	void MultiTargetAction::useActionFor(ObjectPointer target, ActionDescriptor* ad) {
		UseAction::setFor(target, ad, this);
	}
	
	const char * MultiTargetAction::DefaultMessages::unspecifiedMore = "And which one - %?";
	const char * MultiTargetAction::DefaultMessages::unspecifiedOne = "You should specify what next time.";
	const char * MultiTargetAction::DefaultMessages::chosenForYou = "I have chosen % for you.";
	const char * MultiTargetAction::DefaultMessages::dontCare = "If you don't care, I'll just take %.";
	const char * MultiTargetAction::DefaultMessages::uncertain = "Sorry, did you mean %?";
	const char * MultiTargetAction::DefaultMessages::totallyUncertain = "Sorry, I don't know what you mean. Please try to explain it better.";
	const char * MultiTargetAction::DefaultMessages::noCandidate = "No such thing found. Really.";
	const char * MultiTargetAction::DefaultMessages::commonRegex = "it|that";
	const char * MultiTargetAction::DefaultMessages::nextTime = "You should specify what next time.";
}
