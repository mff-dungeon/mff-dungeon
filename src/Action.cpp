#include "Action.hpp"
#include "ActionDescriptor.hpp"

namespace Dungeon {

	void Action::commit(ActionDescriptor* ad) {
		ad->addMessage("Olala! The action was commited!");
	}
	
	bool Action::matchCommand(string command) {
		return true;
	}

	void Action::explain(ActionDescriptor* ad) {
		ad->addMessage("Sorry. Neither me does know what should that mean.");
	}
	
}
