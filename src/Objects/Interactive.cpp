#include "Interactive.hpp"
#include "../Game/ActionDescriptor.hpp"

namespace Dungeon {
	
	void Interactive::registerProperties(IPropertyStorage& storage) {
		storage.have(content, "interactive-content", "Readable content:");
		IDescriptable::registerProperties(storage);
	}

	PERSISTENT_IMPLEMENTATION(Interactive)
			

	Interactive::BasicInteraction::BasicInteraction(string type, string verb, string explanation) 
                : MultiTargetAction(type), verb(verb), explanation(explanation) {
	}

	void Interactive::BasicInteraction::explain(ActionDescriptor* ad) {
		*ad << explanation << eos;
	}

	void Interactive::BasicInteraction::commitOnTarget(ActionDescriptor* ad, ObjectPointer target) {
		target.assertType<Interactive>("Interaction must be done on interactive target.");
		string content = target.unsafeCast<Interactive>()->getContent();
		*ad << ad->formatMessage(content) << eos;
	}

	bool Interactive::BasicInteraction::match(string command, ActionDescriptor* ad) {
		if (RegexMatcher::match(verb + " .+", command)) {
			selectBestTarget(command.substr(verb.length() + 1), ad);
			return true;
		}
		return false;
	}
}

