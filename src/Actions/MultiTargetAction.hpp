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
        
        virtual void merge(MultiTargetAction* second);
        virtual void addTarget(ObjectPointer op);

        const ObjectMap& getTargets() const;

        virtual void commitOnTarget(ActionDescriptor* ad, ObjectPointer target) = 0;
        
    protected:
        ObjectMap targets;
    };
}

#endif	/* MULTITARGETACTION_HPP */

