#ifndef MULTITARGETACTION_HPP
#define	MULTITARGETACTION_HPP

#include "../common.hpp"
#include "../Action.hpp"

namespace Dungeon {
    
    /**
     * Action that applies to more elements, merges 
     * with another actions of the same type 
     * and automatically selects matching target
     */
    class MultiTargetAction : public Action {
    public:
        MultiTargetAction(string type);
        virtual ~MultiTargetAction();
        
        /**
         * Invoked by ActionList, when other action of the same type arrives.
         * Should merge the lists of targets and delete the second action.
         * @param second The other action of the same type
         */
        virtual void merge(MultiTargetAction* second);
        
        /**
         * Adds another potential target for this action.
         * @param op Target
         */
        virtual void addTarget(ObjectPointer op);

        const ObjectMap& getTargets() const;

        /**
         * Like commit in general action, but this time with selected target.
         * @param ad
         * @param target Selected target
         */
        virtual void commitOnTarget(ActionDescriptor* ad, ObjectPointer target) = 0;
        
        /*
         * Once we implement the object matching, we should probably use it here
         * and override the commit function. Probably we would need to strip 
         * the beginning of the command string and pass only the rest.
         */
        
    protected:
        ObjectMap targets;
    };
}

#endif	/* MULTITARGETACTION_HPP */

