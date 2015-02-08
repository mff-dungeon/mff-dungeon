#include "Board.hpp"
#include "../../Game/ActionList.hpp"

namespace Dungeon {
	
	void Board::getActions(ActionList* list, ObjectPointer callee) {
		auto action = new Interactive::BasicInteraction("board-read", "read", 
							"read ... - to read something.");
		action->addTarget(this);
		list->addAction(action);
	}

	PERSISTENT_IMPLEMENTATION(Board)
}

