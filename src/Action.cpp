#include "Action.hpp"

namespace Dungeon {

	void Action::commit(ActionDescriptor* ad) {
		ad->message = "Olala! The action was commited!";
	}
	
	bool Action::matchCommand(string command) {
		return true;
	}

	void Action::explain(ActionDescriptor* ad) {
		ad->message = "Sorry. Neither me does know what should that mean.";
	}
	
}
