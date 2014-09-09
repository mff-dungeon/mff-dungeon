#include "Trap.hpp"
#include "../ActionDescriptor.hpp"

namespace Dungeon {
	
	PERSISTENT_IMPLEMENTATION(Trap)
	
	TrapException::TrapException(const ObjectPointer& trap)
	: GameException("Trap was triggered and driver handled it wrong.") {
		trap.assertExists("Triggered trap does not exist")
				.assertType<Trap>("Something that should be trap is not trap.");

		this->trap = trap;
		this->trap.setLock();
	}
	
	void DemoTrap::trigger(string event, ObjectPointer target, ActionDescriptor* ad) {
		LOG("DemoTrap") << "Trap on " << target.getId() << ", event " << event << " triggered" << LOGF;
		if (ad) 
			*ad << "Trap on " + target.getId() + ", event " + event + " triggered.";
	}	
			
}

