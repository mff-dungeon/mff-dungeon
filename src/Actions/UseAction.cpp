#include "UseAction.hpp"
#include "../RegexMatcher.hpp"
#include "../ActionList.hpp"

namespace Dungeon {

	bool UseAction::match(string command, ActionDescriptor* ad) {
		smatch matches;
		if (RegexMatcher::match("(use|utilize|apply) (.+)", command, matches)) {
			selectBestTarget(matches[2], ad);
			return true;
		}
		return false;
	}
	
	void UseAction::setFor(ObjectPointer target, ActionList* list, MultiTargetAction* action) {
		((UseAction*) list->addAction(new UseAction))
				->addTarget(target, action);
	}

}

