#include "BasicInteraction.hpp"
#include "../../Game/ActionDescriptor.hpp"

namespace Dungeon {

	void BasicInteraction::trigger(ObjectPointer target, ActionDescriptor* ad) {
		if (!ad) throw InvalidEvent("This behavior needs AD.");
		*ad << ad->formatMessage(content) << eos;
	}
	
	void BasicInteraction::registerProperties(IPropertyStorage& storage) {
		storage.have(content, "interactive-content", "Readable content:");
		Behavior::registerProperties(storage);
	}
	
	PERSISTENT_IMPLEMENTATION(BasicInteraction)
}

