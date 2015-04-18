#include "MultiTargetAction.hpp"
#include "UseAction.hpp"
#include "../Utils/FuzzyStringMatcher.hpp"
#include "../Game/ObjectPointer.hpp"
#include "../Game/ObjectGroup.hpp"
#include "../Game/ActionDescriptor.hpp"
#include "../Objects/Traps/MTATrap.hpp"

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
		ObjectPointer trap = ad->getGM()->getObject("trap/mta");
		setTarget(trap.unsafeCast<MTATrap>()->wrapFind(targets, this, str, ad));
	}

	void MultiTargetAction::useActionFor(ObjectPointer target, ActionDescriptor* ad) {
		UseAction::setFor(target, ad, this);
	}
}
