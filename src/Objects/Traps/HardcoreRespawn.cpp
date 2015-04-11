#include "HardcoreRespawn.hpp"

namespace Dungeon {

	/*
	 * TODO: workout a way to deal with old characters and replace them with new characters
	 */
	void HardcoreRespawn::exceptionTrigger(ActionDescriptor* ad) {
		Trap::exceptionTrigger(ad);
	}

	void HardcoreRespawn::trigger(const string& event, ObjectPointer target, ActionDescriptor* ad) {
		throw TrapException(this);
	}

	PERSISTENT_IMPLEMENTATION(HardcoreRespawn)
}