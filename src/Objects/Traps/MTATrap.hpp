#ifndef FUZZYMATCHTRAP_HPP
#define	FUZZYMATCHTRAP_HPP

#include "../../common.hpp"
#include "../../Game/ActionDescriptor.hpp"
#include "../../Actions/MultiTargetAction.hpp"
#include "Trap.hpp"

namespace Dungeon {

	/**
	 * Violating the rules, this trap is used to fetch additional information
	 * when the match is too fuzzy.
	 */
	class MTATrap : public Trap {
	public:
		MTATrap() : Trap() { }
		MTATrap(objId id) : Trap(id) { }
		virtual ~MTATrap() { }

		enum Phase {
			Selecting,
			Return,
			Cancel
		};

		virtual ObjectPointer wrapFind(ObjectMap group, MultiTargetAction* action, string str, ActionDescriptor* ad);

		/**
		 * It's fucking magic :) Can explain personally if you want.
		 * @param ad
		 */
		virtual bool exceptionTrigger(ActionDescriptor* ad);

	private:
		ObjectMap objects;
		Phase phase = Selecting;
		ObjectPointer target;

		PERSISTENT_DECLARATION(MTATrap, Trap)
	};
}

#endif

