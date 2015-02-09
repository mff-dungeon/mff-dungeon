#include "Interactive.hpp"
#include "../Game/ActionDescriptor.hpp"
#include "../Game/GameManager.hpp"
#include <string>

using namespace std;

namespace Dungeon {
	
	void Interactive::attachInteraction(const string& verb, const string& explanation, const string& content) {
		BasicInteraction* trap = new BasicInteraction("trap/interaction/" + getId() + "/" + RANDID);
		trap->setVerb(verb)
			->setExplanation(explanation)
			->setContent(content);
		getGameManager()->insertObject(trap);

		attachTrap(trap, "action-interaction-" + verb);
		attachTrap(trap, "get-actions");
	}

	PERSISTENT_IMPLEMENTATION(Interactive)
		
	Interactive::BasicInteractionAction::BasicInteractionAction(const string& type, ObjectPointer interactionTrap) 
                : MultiTargetAction(type), interactionTrap(interactionTrap) {
		interactionTrap.assertType<BasicInteraction>();
	}

	void Interactive::BasicInteractionAction::explain(ActionDescriptor* ad) {
		*ad << interactionTrap.unsafeCast<BasicInteraction>()->getExplanation() << eos;
	}

	void Interactive::BasicInteractionAction::commitOnTarget(ActionDescriptor* ad, ObjectPointer target) {
		// Everything is handled by the trap itself.
	}

	bool Interactive::BasicInteractionAction::match(const string& command, ActionDescriptor* ad) {
		auto it = interactionTrap.unsafeCast<BasicInteraction>();
		smatch matches;
		if (RegexMatcher::match(it->getVerb() + " +(.+)", command, matches)) {
			selectBestTarget(matches[matches.size() - 1], ad);
			return true;
		}
		return false;
	}
	
	void Interactive::BasicInteraction::trigger(const string& event, ObjectPointer target, ActionDescriptor* ad) {
		if (event == "get-actions") {
			if (!target->hasTrapAttached(this, "action-interaction-" + verb))
				throw GameStateInvalid("BasicInteraction trap not properly attached.");
			
			auto action = new Interactive::BasicInteractionAction("interaction-" + verb, this);
			action->addTarget(target);
			delete action;
			/* FUCK. I need to add the action here. */
		} else if (event == "action-interaction-" + verb) {
			if (!ad) throw InvalidEvent();
			*ad << ad->formatMessage(content) << eos;
		}
	}
	
	void Interactive::BasicInteraction::registerProperties(IPropertyStorage& storage) {
		storage.have(content, "interactive-content", "Readable content:")
			.have(verb, "interactive-verb", "Verb that triggers the interaction:")
			.have(explanation, "interactive-explanation", "Explanation of this action:");
		Trap::registerProperties(storage);
	}
	
	PERSISTENT_IMPLEMENTATION_FULL(Interactive::BasicInteraction, BasicInteraction)

}

