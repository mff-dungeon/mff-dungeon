#include "HardcoreRespawn.hpp"

namespace Dungeon {
	/*
	 * TODO: workout a way to deal with old characters and replace them with new characters
     */
	bool HardcoreRespawn::exceptionTrigger(ActionDescriptor* ad) {
		return false;
	}

	void HardcoreRespawn::trigger(string event, ObjectPointer target, ActionDescriptor* ad) {
		throw TrapException(this);
	}

	PERSISTENT_IMPLEMENTATION(HardcoreRespawn)
}