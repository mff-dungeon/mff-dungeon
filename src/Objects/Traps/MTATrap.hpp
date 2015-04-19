#ifndef FUZZYMATCHTRAP_HPP
#define	FUZZYMATCHTRAP_HPP

#include <regex>
#include "../../common.hpp"
#include "../../Utils/SentenceJoiner.hpp"
#include "../../Game/ActionDescriptor.hpp"
#include "../../Game/ObjectGroup.hpp"
#include "../Virtual/IDescriptable.hpp"
#include "Trap.hpp"

namespace Dungeon {

        class MultiTargetAction;
        
	/**
	 * Violating the rules, this trap is used to fetch additional information
	 * when the match is too fuzzy.
	 */
	class MTATrap : public Trap {
	public:
                
		MTATrap() : Trap() { }
		MTATrap(const objId& id) : Trap(id) { }
		virtual ~MTATrap() { }

		enum Phase {
			Selecting,
			Return,
			Cancel
		};

		ObjectPointer wrapFind(ObjectMap group, MultiTargetAction* action, const string& str, ActionDescriptor* ad);

		/**
		 * It's fucking magic :) Can explain personally if you want.
		 * @param ad
		 */
		virtual void exceptionTrigger(ActionDescriptor* ad);

	private:
		ObjectMap objects;
		Phase phase = Selecting;
		ObjectPointer target;

		PERSISTENT_DECLARATION(MTATrap, Trap)
	};
        
}

#endif

