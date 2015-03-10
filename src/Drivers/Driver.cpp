#include "Driver.hpp"
#include "../Game/ActionDescriptor.hpp"
#include "../Game/ActionQueue.hpp"

namespace Dungeon {

	Driver::Driver(ActionQueue* queue) : queue(queue) {
	}

	void Driver::processDescriptor(ActionDescriptor* descriptor) {
		if (typeid (*descriptor) == typeid (TextActionDescriptor))
			LOGS("Driver", Debug) << "Action descriptor arrived: '" << ((TextActionDescriptor*) descriptor)->in_msg << "'." << LOGF;
		else
			LOGS("Driver", Debug) << "Action descriptor arrived." << LOGF;

		delete descriptor;
	}

}

