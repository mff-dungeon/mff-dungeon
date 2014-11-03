#include "UseAction.hpp"
#include "../Utils/RegexMatcher.hpp"
#include "../Game/ActionDescriptor.hpp"
#include "../Game/ActionList.hpp"

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

	void UseAction::explain(ActionDescriptor* ad) {
		*ad << "use ... - Uses the object." << eos;
	}

}

