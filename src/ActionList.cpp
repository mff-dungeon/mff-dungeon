#include "ActionList.hpp"

namespace Dungeon {
	
	ActionList::ActionList() {
	}

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
			return action;
		} else {
			((MultiTargetAction*) it->second)->merge(action);
			action->remember()->forget(); // Dispose if not used
			return (MultiTargetAction*) it->second;
		}
	}
	
	Action* ActionList::addAction(Action* action) {
		insert(std::make_pair(action->type, action));
		action->remember();
		return action;
	}
	
}
