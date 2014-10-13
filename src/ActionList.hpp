#ifndef ACTIONLIST_HPP
#define	ACTIONLIST_HPP

#include "common.hpp"
#include "Action.hpp"
#include "Actions/MultiTargetAction.hpp"

namespace Dungeon {
    typedef std::multimap<string, Action*> actionMap;
        
    /**
     * Sums up all actions that can be performed. 
     * Takes care of merging actions of the same type.
     */
    class ActionList : public actionMap {
    public:
        ActionList();
        ActionList(const ActionList& orig);
        virtual ~ActionList();
        
        Action* addAction(Action* action);
        MultiTargetAction* addAction(MultiTargetAction* action);
        void clear();
    };
}

#endif

