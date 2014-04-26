#include "CallbackAction.hpp"

namespace Dungeon {
	void CallbackAction::explain(ActionDescriptor* ad) {
		ad->message = explanation;
	}

	void CallbackAction::commit(ActionDescriptor* ad) {
		commitCallback(ad);
	}

	bool CallbackAction::matchCommand(string command) {
		return matchCallback(command);
	}
}