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
                 
		enum Phase {
			Selecting,
			Return,
			Cancel
		};
                
        struct Info {
            ObjectMap objects;
            Phase phase = Selecting;
            ObjectPointer target;
        };
        
        struct Messages {
            const char *unspecified = "And which one - %?";
            const char *dontCare = "If you don't care, I'll just take %.";
            const char *uncertain = "Sorry, did you mean %?";
            const char *noCandidate = "Sorry, I don't know what you mean. Please try to explain it better.";
            const char *nextTime = "You should specify what next time.";
            const char *itRegex = "it|that";
            const char *anyRegex = "(any|some)(\\s+(.*))?;";
            /** if matched, searchstring will be replaced with this match */
            size_t anyRegexMatchIndex = 3;
		};
                
		MTATrap() : Trap() { }
		MTATrap(const objId& id) : Trap(id) { }
		virtual ~MTATrap() { }

		ObjectPointer wrapFind(ObjectMap group, ActionDescriptor* ad, const string& str);

		/**
		 * It's fucking magic :) Can explain personally if you want.
		 * @param ad
		 */
		virtual void exceptionTrigger(ActionDescriptor* ad);

		PERSISTENT_DECLARATION(MTATrap, Trap)
	};
        
}

#endif

