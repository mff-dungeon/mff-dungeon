#ifndef ACTIONLIST_HPP
#define	ACTIONLIST_HPP

#include "../common.hpp"
#include "../Actions/Action.hpp"

namespace Dungeon {
    typedef std::multimap<string, Action*> actionMap;

    class MultiTargetAction;
    
    /**
     * Sums up all actions that can be performed.
     * Takes care of merging actions of the same type.
     */
    class ActionList : public actionMap {
    public:
        ActionList();
        virtual ~ActionList();

        Action* addAction(Action* action);
        MultiTargetAction* addAction(MultiTargetAction* action);
        void clear();
    };
}

#endif

