#ifndef USEACTION_HPP
#define	USEACTION_HPP

#include "../common.hpp"
#include "Polyaction.hpp"

namespace Dungeon {

	class UseAction : public Polyaction {
	public:
		UseAction() : Polyaction("use") {
                    // Use should not be used without a target. It is too dangerous.
                    messages.uncertain = "Sorry, did you want to use %?";
                    messages.noCandidate = "Sorry, I don't understand what to use. Please try to explain it better.";
                };
		virtual ~UseAction() { }

		virtual bool match(const string& command, ActionDescriptor* ad);
		virtual void explain(ActionDescriptor* ad);

		static void setFor(ObjectPointer target, ActionDescriptor* ad, MultiTargetAction* action);
	};
}

#endif

