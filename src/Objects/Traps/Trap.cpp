#include "Trap.hpp"
#include "../../Game/ActionDescriptor.hpp"

namespace Dungeon {

	PERSISTENT_IMPLEMENTATION(Trap)

	TrapException::TrapException(const ObjectPointer& trap)
	: GameException("Trap was triggered and driver handled it wrong.") {
		trap.assertExists("Triggered trap does not exist")
				.assertType<Trap>("Something that should be trap is not trap.");

		this->trap = trap;
		this->trap.setLock();
	}

	void Trap::exceptionTrigger(ActionDescriptor* ad) {
		ad->state = ActionDescriptor::Finished;
	}

	ObjectPointer TrapException::getTrap() const {
		return trap;
	}

	PERSISTENT_IMPLEMENTATION(DemoTrap)

	void DemoTrap::trigger(const string& event, ObjectPointer target, ActionDescriptor* ad) {
		LOGS(Debug) << "Trap on " << target.getId() << ", event " << event << " triggered" << LOGF;
		if (ad)
			*ad << " [" + target.getId() + " // " + event + "]" << eos;
	}


}

