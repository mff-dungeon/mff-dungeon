#include "CallbackAction.hpp"
#include "../ActionDescriptor.hpp"

namespace Dungeon {
	void CallbackAction::explain(ActionDescriptor* ad) {
		ad->addMessage(explanation);
	}

	void CallbackAction::commit(ActionDescriptor* ad) {
		commitCallback(ad);
	}

	bool CallbackAction::matchCommand(string command) {
		return matchCallback(command);
	}
}