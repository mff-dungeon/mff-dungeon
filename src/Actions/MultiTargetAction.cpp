#include "MultiTargetAction.hpp"
#include "../ObjectPointer.hpp"

namespace Dungeon {

	MultiTargetAction::~MultiTargetAction() {
		for (auto& pair : targets) {
			delete pair.second;
		}
	}

	MultiTargetAction::MultiTargetAction(string type) : Action(type) {}

	void MultiTargetAction::addTarget(ObjectPointer* op) {
		if (targets.find(op->getId()) == targets.end())
			targets[op->getId()] = op->clone();
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
	
}
