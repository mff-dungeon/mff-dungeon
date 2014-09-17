#include "Polyaction.hpp"

namespace Dungeon {
	
	MultiTargetAction* Polyaction::addTarget(ObjectPointer op) {
		throw new GameException("You can't add just target to Polyaction, you must provide the action too.");
	}
	
	Polyaction* Polyaction::addTarget(ObjectPointer op, MultiTargetAction* action) {
		MultiTargetAction::addTarget(op);
		action->remember(); // If action == actionMap[id], it could be deleted in forget
		if (actionMap.find(op.getId()) != actionMap.end())
				actionMap[op.getId()]->forget();
		actionMap[op.getId()] = action;
		return this;
	}
		
	void Polyaction::commitOnTarget(ActionDescriptor* ad, ObjectPointer target) {
		MultiTargetAction* action = actionMap[target.getId()];
		action->setTarget(target);
		action->commit(ad);
	}
	
	void Polyaction::merge(MultiTargetAction* second) {
		Polyaction* pol = (Polyaction*) second;
		for (auto& pair : pol->getTargets()) {
			addTarget(pair.second, pol);
		}
		// second action is not deleted, as it's used to delegate calling :)
	}


	Polyaction::~Polyaction() {
		for (auto& pair : actionMap) {
			pair.second->forget();
		}
	}



}
