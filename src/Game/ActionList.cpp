#include "ActionList.hpp"
#include "../Actions/MultiTargetAction.hpp"

namespace Dungeon {

	ActionList::ActionList() { }

	ActionList::~ActionList() {
		clear();
	}

	void ActionList::clear() {
		for (actionMap::iterator it = begin(); it != end(); it++) {
			it->second->forget();
		}
		actionMap::clear();
	}

	MultiTargetAction* ActionList::addAction(MultiTargetAction* action) {
		actionMap::iterator it = find(action->type);
		if (it == end()) {
			insert(std::make_pair(action->type, action));
			action->remember();
			LOGS(Debug) << "Added a multitarget action of type " << action->type << "." << LOGF;
			return action;
		} else {
			((MultiTargetAction*) it->second)->merge(action);
			LOGS(Debug) << "Added and merged a multitarget action of type " << action->type << "." << LOGF;
			action->remember()->forget(); // Dispose if not used
			return (MultiTargetAction*) it->second;
		}
	}

	Action* ActionList::addAction(Action* action) {
		insert(std::make_pair(action->type, action));
		action->remember();
		LOGS(Debug) << "Added an action of type " << action->type << "." << LOGF;
		return action;
	}

}
