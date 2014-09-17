#include "MultiTargetAction.hpp"
#include "../ObjectPointer.hpp"
#include "../ObjectGroup.hpp"
#include "../FuzzyStringMatcher.hpp"
#include "../ActionDescriptor.hpp"
#include "../Traps/MTATrap.hpp"

namespace Dungeon {

	MultiTargetAction::~MultiTargetAction() {
	}

	MultiTargetAction::MultiTargetAction(string type) : Action(type) {
	}

	void MultiTargetAction::addTarget(ObjectPointer op) {
		if (targets.find(op.getId()) == targets.end())
			targets[op.getId()] = op;
	}
	
	void MultiTargetAction::merge(MultiTargetAction* second) {
		for (auto& pair : second->getTargets()) {
			addTarget(pair.second);
		}
		delete second;
	}
	
	const ObjectMap& MultiTargetAction::getTargets() const {
		return targets;
	}
	
	void MultiTargetAction::commit(ActionDescriptor* ad) {
		commitOnTarget(ad, selectedTarget);
	}

	void MultiTargetAction::selectBestTarget(string str, ActionDescriptor* ad) {
		ObjectGroup group (targets);
		ObjectPointer trap = ad->getGM()->getObject("trap/mta");
		setTarget(trap.unsafeCast<MTATrap>()->wrapFind(group, this, str, ad));
	}
}
