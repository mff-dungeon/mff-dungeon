#include "ActionList.hpp"

namespace Dungeon {
	
	ActionList::ActionList() {
	}

	ActionList::~ActionList() {
		clear();
	}
	
	void ActionList::clear() {
		for (actionMap::iterator it = begin(); it != end(); it++) {
            delete it->second;
		}
		actionMap::clear();
	}
	
	Action* ActionList::addAction(MultiTargetAction* action) {
		actionMap::iterator it = find(action->type);
		if (it == end()) {
			insert(std::make_pair(action->type, action));
			return action;
		} else {
			((MultiTargetAction*) it->second)->merge(action);
			return it->second;
		}
	}
	
	Action* ActionList::addAction(Action* action) {
		insert(std::make_pair(action->type, action));
		return action;
	}
	
}
