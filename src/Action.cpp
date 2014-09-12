#include "Action.hpp"
#include "ActionDescriptor.hpp"

namespace Dungeon {

	void Action::commit(ActionDescriptor* ad) {
		*ad << "Olala! The action was commited!" << eos;
	}
	
	bool Action::matchCommand(string command) {
		return true;
	}

	void Action::explain(ActionDescriptor* ad) {
		*ad << "Sorry. Neither me does know what should that mean." << eos;
	}
	
	bool Action::handleException(GameException& exception, ActionDescriptor* ad) {
		*ad << "Sorry, an unexpected in-game error occured. Administrator have already been notified.\n";
		*ad << exception.what() << eos;
		return true;
	}

}
