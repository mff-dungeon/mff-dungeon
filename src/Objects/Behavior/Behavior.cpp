#include <regex>
#include "Behavior.hpp"
#include "../../Game/ActionDescriptor.hpp"
#include "../../exceptions.hpp"

namespace Dungeon {

	void Behavior::registerProperties(IPropertyStorage& storage) {
		storage.have(verb, "interactive-verb", "Verb that triggers the behavior:")
			.have(explanation, "interactive-explanation", "Explanation of this behavior:");
		Base::registerProperties(storage);
	}

	void Behavior::getActions(ActionDescriptor* ad) {
		auto& target = ad->delegationStack.top();
		LOGS(Debug) << "Adding behavior " << verb << " on " << target << LOGF;
		auto action = new BehaviorAction("behavior-" + verb, this);
		action->addTarget(target);
		ad->getActionList().addAction(action);
	}

	BehaviorAction::BehaviorAction(const string& type, ObjectPointer behavior)
                : MultiTargetAction(type), behavior(behavior) {
		behavior.assertType<Behavior>();
	}

	void BehaviorAction::explain(ActionDescriptor* ad) {
		*ad << behavior.unsafeCast<Behavior>()->getExplanation() << eos;
	}

	void BehaviorAction::commitOnTarget(ActionDescriptor* ad, ObjectPointer target) {
		behavior.unsafeCast<Behavior>()->trigger(target, ad);
	}

	bool BehaviorAction::match(const string& command, ActionDescriptor* ad) {
		auto b = behavior.unsafeCast<Behavior>();
		smatch matches;
		if (RegexMatcher::match(b->getVerb() + "( +(.+))?", command, matches)) {
			selectBestTarget(matches[matches.size() - 1], ad);
			return true;
		}
		return false;
	}

	NONPERSISTENT_IMPLEMENTATION(Behavior, Behavior)
}

