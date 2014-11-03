#include "CallbackAction.hpp"
#include "../Game/ActionDescriptor.hpp"

namespace Dungeon {

	void CallbackAction::explain(ActionDescriptor* ad) {
		*ad << explanation << eos;
	}

	void CallbackAction::commit(ActionDescriptor* ad) {
		commitCallback(ad);
	}

	bool CallbackAction::match(string command, ActionDescriptor* ad) {
		return matchCallback(command);
	}

}