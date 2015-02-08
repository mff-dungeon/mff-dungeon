#include "Speaker.hpp"
#include "../../Game/ActionList.hpp"

namespace Dungeon {
	
	void Speaker::getActions(ActionList* list, ObjectPointer callee) {
		auto action = new Interactive::BasicInteraction("speaker-speak", "speak", 
							"speak with ... - to speak with someone in game");
		action->addTarget(this);
		list->addAction(action);
	}

	PERSISTENT_IMPLEMENTATION(Speaker)
}

