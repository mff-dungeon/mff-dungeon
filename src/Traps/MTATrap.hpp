#ifndef FUZZYMATCHTRAP_HPP
#define	FUZZYMATCHTRAP_HPP

#include "../common.hpp"
#include "Trap.hpp"
#include "../ObjectGroup.hpp"

namespace Dungeon
{

        /**
         * Violating the rules, this trap is used to fetch additional information
         * when the match is too fuzzy.
         */
	class MTATrap : public Trap
	{
	public:
            MTATrap() : Trap() {}
            MTATrap(string id) : Trap(id) {}
            virtual ~MTATrap() {}
            
            enum Phase {
                Selecting,
                Return,
                Cancel
            };

            virtual ObjectPointer wrapFind(ObjectGroup group, MultiTargetAction* action, string str, ActionDescriptor* ad);

            /**
             * It's fucking magic :) Can explain personally if you want.
             * @param ad
             */
            virtual bool exceptionTrigger(ActionDescriptor* ad);
            
        private:
            ObjectGroup group;
            Phase phase = Selecting;
            ObjectPointer target;

            PERSISTENT_DECLARATION(MTATrap, Trap)
	};
        
        class FuzzyMatchTrapAction : public Action {
            
        };
}

#endif

