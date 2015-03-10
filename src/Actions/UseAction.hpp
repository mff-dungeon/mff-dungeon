#ifndef USEACTION_HPP
#define	USEACTION_HPP

#include "../common.hpp"
#include "Polyaction.hpp"

namespace Dungeon {

	class UseAction : public Polyaction {
	public:
		UseAction() : Polyaction("use") { };
		virtual ~UseAction() { }

		virtual bool match(const string& command, ActionDescriptor* ad);
		virtual void explain(ActionDescriptor* ad);

		static void setFor(ObjectPointer target, ActionList* list, MultiTargetAction* action);
	};
}

#endif

